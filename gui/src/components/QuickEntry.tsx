import { useState } from "react";
import { Card, CardHeader, CardContent } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Zap } from "lucide-react";

export function QuickEntry() {
  const [instrument, setInstrument] = useState("Rose");
  const [price, setPrice] = useState("");
  const [quantity, setQuantity] = useState("");

  const instruments = ["Rose", "Lavender", "Lotus", "Tulip", "Orchid"];

  return (
    <Card className="border-border bg-card w-full h-full">
      <CardHeader className="py-4 px-5 bg-muted/50 border-b border-border flex flex-row items-center gap-2">
        <Zap size={18} className="text-yellow-500" />
        <h3 className="text-md font-bold tracking-wide text-foreground">
          Quick Entry
        </h3>
      </CardHeader>
      
      <CardContent className="p-5 flex flex-col gap-6 h-[calc(100%-65px)]">
        <div className="flex flex-col gap-4">
          <div className="flex flex-col gap-1">
            <span className="text-sm font-medium text-muted-foreground">Instrument</span>
            <select 
              value={instrument}
              onChange={(e) => setInstrument(e.target.value)}
              className="bg-background border border-border rounded-lg p-2 text-foreground text-sm focus:outline-none focus:ring-1 focus:ring-ring"
            >
              {instruments.map((inst) => (
                <option key={inst} value={inst} className="bg-background text-foreground">
                  {inst}
                </option>
              ))}
            </select>
          </div>

          <div className="flex gap-3">
            <div className="flex flex-col gap-1 w-1/2">
              <span className="text-sm font-medium text-muted-foreground">Price</span>
              <div className="flex items-center bg-background border border-border rounded-lg overflow-hidden focus-within:ring-1 focus-within:ring-ring">
                <span className="pl-3 text-muted-foreground text-sm">$</span>
                <input 
                  type="number" 
                  placeholder="0.00" 
                  className="bg-transparent text-foreground text-sm p-2 w-full focus:outline-none"
                  value={price}
                  onChange={(e) => setPrice(e.target.value)}
                />
              </div>
            </div>            
            <div className="flex flex-col gap-1 w-1/2">
               <span className="text-sm font-medium text-muted-foreground">Quantity</span>
               <input 
                  type="number" 
                  placeholder="100" 
                  className="bg-background border border-border rounded-lg text-foreground text-sm p-2 w-full focus:outline-none focus:ring-1 focus:ring-ring"
                  value={quantity}
                  onChange={(e) => setQuantity(e.target.value)}
                />
            </div>
          </div>
        </div>

        <hr className="border-border my-1" />

        <div className="grid grid-cols-2 gap-3 mt-auto">
          <Button 
            className="bg-green-500/20 text-green-500 hover:bg-green-500 hover:text-green-950 font-bold tracking-wider transition-all"
            size="lg"
            variant="outline"
            onClick={() => console.log("BUY", { instrument, price, quantity })}
          >
            BUY
          </Button>
          <Button 
            className="bg-red-500/20 text-red-500 hover:bg-red-500 hover:text-white font-bold tracking-wider transition-all"
            size="lg"
            variant="outline"
            onClick={() => console.log("SELL", { instrument, price, quantity })}
          >
            SELL
          </Button>
        </div>
      </CardContent>
    </Card>
  );
}
