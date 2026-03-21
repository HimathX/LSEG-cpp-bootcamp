from fastapi import FastAPI, UploadFile, File, HTTPException
from fastapi.middleware.cors import CORSMiddleware
import subprocess
import os
import shutil

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
OUTPUT_FILE = "execution_rep.csv"

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

        # 3. Check if the engine generated the execution report
        if not os.path.exists(OUTPUT_FILE):
            raise HTTPException(status_code=500, detail="Engine failed to generate execution_rep.csv")

        # 4. Read the report data
        with open(OUTPUT_FILE, "r") as f:
            csv_content = f.read()

        # 5. Return the summary (stdout) and the raw CSV data to the frontend
        return {
            "success": True,
            "summary": result.stdout, # The counts you print in your C++ main.cpp
            "data": csv_content
        }

    except subprocess.CalledProcessError as e:
        return {"success": False, "error": f"Engine Crash: {e.stderr}"}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/health")
def health_check():
    return {"status": "online", "engine_found": os.path.exists(ENGINE_PATH)}

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="127.0.0.1", port=8001)