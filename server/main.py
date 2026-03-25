from fastapi import FastAPI, UploadFile, File, HTTPException
from fastapi.responses import FileResponse
from fastapi.middleware.cors import CORSMiddleware
import subprocess
import os
import shutil
import csv
app = FastAPI(title="Flower Exchange Bridge")

# --- CORS CONFIGURATION ---
# This allows your React frontend (usually on port 5173) to talk to this API
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"], # In production, restrict this to your frontend URL
    allow_methods=["*"],
    allow_headers=["*"],
)

# Paths to your engine and data files
ENGINE_PATH = os.path.abspath("../exchange.exe")
INPUT_FILE = "orders.csv"
SUCCESS_OUTPUT_FILE = "execution_rep.csv"
REJECTED_OUTPUT_FILE = "rejected_execution_rep.csv"

def parse_csv_to_dicts(filepath):
    reports = []
    if not os.path.exists(filepath):
        return reports
    with open(filepath, "r") as f:
        reader = csv.reader(f)
        header = next(reader, None)
        for row in reader:
            if len(row) < 9: continue
            side_text = row[3].strip().lower()
            side = 1 if side_text in ("buy", "1") else 2
            try:
                price_val = float(row[4].strip())
            except ValueError:
                price_val = 0.0

            try:
                qty_val = int(row[5].strip())
            except ValueError:
                qty_val = 0

            try:
                status_val = int(row[6].strip())
            except ValueError:
                status_val = 0
            
            reports.append({
                "orderId": row[0].strip(),
                "clientOrderId": row[1].strip(),
                "instrument": row[2].strip(),
                "side": side,
                "price": price_val,
                "quantity": qty_val,
                "status": status_val,
                "reason": row[7].strip() if row[7].strip() else None,
                "transactionTime": row[8].strip()
            })
    return reports

@app.post("/api/execute")
async def execute_matching_engine(file: UploadFile = File(...)):
    """
    Receives a CSV file, runs the C++ exchange.exe, and returns the results.
    """
    try:
        # 1. Save the uploaded file locally as orders.csv
        with open(INPUT_FILE, "wb") as buffer:
            shutil.copyfileobj(file.file, buffer)

        # 2. Run the C++ engine (Optimized -O3 binary)
        # We use check=True to raise an error if the engine crashes
        result = subprocess.run(
            [ENGINE_PATH, INPUT_FILE], 
            capture_output=True, 
            text=True, 
            check=True
        )

        # 3. Check if the engine generated both execution reports
        if not os.path.exists(SUCCESS_OUTPUT_FILE):
            raise HTTPException(status_code=500, detail="Engine failed to generate execution_rep.csv")

        if not os.path.exists(REJECTED_OUTPUT_FILE):
            raise HTTPException(status_code=500, detail="Engine failed to generate rejected_execution_rep.csv")

        # 4. Read and parse both report files
        execution_data = parse_csv_to_dicts(SUCCESS_OUTPUT_FILE)
        rejected_data = parse_csv_to_dicts(REJECTED_OUTPUT_FILE)

        # 5. Return the summary (stdout) and both parsed JSON outputs to the frontend
        return {
            "success": True,
            "summary": result.stdout, # The counts you print in your C++ main.cpp
            "data": execution_data,
            "executionData": execution_data,
            "rejectedData": rejected_data,
        }

    except subprocess.CalledProcessError as e:
        return {"success": False, "error": f"Engine Crash: {e.stderr}"}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/api/download/executions")
def download_executions():
    if not os.path.exists(SUCCESS_OUTPUT_FILE):
        raise HTTPException(status_code=404, detail="File not found")
    return FileResponse(SUCCESS_OUTPUT_FILE, media_type="text/csv", filename="execution_rep.csv")

@app.get("/api/download/rejections")
def download_rejections():
    if not os.path.exists(REJECTED_OUTPUT_FILE):
        raise HTTPException(status_code=404, detail="File not found")
    return FileResponse(REJECTED_OUTPUT_FILE, media_type="text/csv", filename="rejected_execution_rep.csv")

@app.get("/health")
def health_check():
    return {"status": "online", "engine_found": os.path.exists(ENGINE_PATH)}

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="127.0.0.1", port=8001)
