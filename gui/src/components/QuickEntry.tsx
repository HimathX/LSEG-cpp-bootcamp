import { useState } from "react";
import { Card, CardHeader, CardContent as CardBody, Button } from "@heroui/react";
import { Zap } from "lucide-react";

export function QuickEntry() {
  const [instrument, setInstrument] = useState("Rose");
  const [price, setPrice] = useState("");
  const [quantity, setQuantity] = useState("");

  const instruments = ["Rose", "Lavender", "Lotus", "Tulip", "Orchid"];

  return (
    <Card className="bg-zinc-900 border border-white/10 w-full h-full">
      <CardHeader className="py-4 px-5 bg-black/20 border-b border-white/5 flex items-center gap-2">
        <Zap size={18} className="text-warning" />
        <h3 className="text-md font-bold tracking-wide text-slate-200">
          Quick Entry
        </h3>
      </CardHeader>
      
      <CardBody className="p-5 flex flex-col gap-6">
        <div className="flex flex-col gap-4">
          <div className="flex flex-col gap-1">
            <span className="text-sm font-medium text-slate-400">Instrument</span>
            <select 
              value={instrument}
              onChange={(e) => setInstrument(e.target.value)}
              className="bg-black/20 border border-white/10 rounded-lg p-2 text-slate-200 text-sm focus:outline-none focus:border-white/30"
            >
              {instruments.map((inst) => (
                <option key={inst} value={inst} className="bg-zinc-900 text-slate-200">
                  {inst}
                </option>
              ))}
            </select>
          </div>

          <div className="flex gap-3">
            <div className="flex flex-col gap-1 w-1/2">
              <span className="text-sm font-medium text-slate-400">Price</span>
              <div className="flex items-center bg-black/20 border border-white/10 rounded-lg overflow-hidden focus-within:border-white/30">
                <span className="pl-3 text-slate-500 text-sm">$</span>
                <input 
                  type="number" 
                  placeholder="0.00" 
                  className="bg-transparent text-slate-200 text-sm p-2 w-full focus:outline-none"
                  value={price}
                  onChange={(e) => setPrice(e.target.value)}
                />
              </div>
            </div>            
            <div className="flex flex-col gap-1 w-1/2">
               <span className="text-sm font-medium text-slate-400">Quantity</span>
               <input 
                  type="number" 
                  placeholder="100" 
                  className="bg-black/20 border border-white/10 rounded-lg text-slate-200 text-sm p-2 w-full focus:outline-none focus:border-white/30"
                  value={quantity}
                  onChange={(e) => setQuantity(e.target.value)}
                />
            </div>
          </div>
        </div>

        <hr className="border-white/5 my-1" />

        <div className="grid grid-cols-2 gap-3 mt-auto">
          <Button 
            className="bg-[#10b981]/20 text-[#10b981] border border-[#10b981]/50 font-bold tracking-wider hover:bg-[#10b981] hover:text-black transition-all"
            size="lg"
            onClick={() => console.log("BUY", { instrument, price, quantity })}
          >
            BUY
          </Button>
          <Button 
            className="bg-[#f43f5e]/20 text-[#f43f5e] border border-[#f43f5e]/50 font-bold tracking-wider hover:bg-[#f43f5e] hover:text-white transition-all"
            size="lg"
            onClick={() => console.log("SELL", { instrument, price, quantity })}
          >
            SELL
          </Button>
        </div>
      </CardBody>
    </Card>
  );
}
