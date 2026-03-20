import { Card, CardContent, CardHeader, Separator as Divider } from "@heroui/react";

export interface OrderLevel {
  price: number;
  qty: number;
}

interface OrderBookProps {
  instrument: string;
  bids: OrderLevel[]; // Best Buy
  asks: OrderLevel[]; // Best Sell
}

export function OrderBook({ instrument, bids, asks }: OrderBookProps) {
  // Ensure we always have exactly 3 levels to display
  const padLevels = (levels: OrderLevel[]) => {
    const padded = [...levels];
    while (padded.length < 3) {
      padded.push({ price: 0, qty: 0 });
    }
    return padded.slice(0, 3);
  };

  const displayBids = padLevels(bids);
  const displayAsks = padLevels(asks);

  return (
    <Card className="bg-zinc-900 border border-white/10 w-full min-w-[200px]">
      <CardHeader className="py-2 px-3 flex justify-center bg-black/40">
        <h3 className="text-sm font-bold tracking-wide text-slate-300 uppercase">
          {instrument}
        </h3>
      </CardHeader>
      <Divider className="bg-white/10" />
      <CardContent className="p-0">
        <div className="grid grid-cols-2 text-xs text-center border-b border-white/5 bg-black/20">
          <div className="py-1 text-success font-semibold tracking-wide">BID</div>
          <div className="py-1 text-danger font-semibold tracking-wide border-l border-white/5">ASK</div>
        </div>
        
        <div className="flex flex-col font-mono text-sm leading-tight">
          {displayBids.map((bid, i) => {
            const ask = displayAsks[i];
            return (
              <div key={i} className="grid grid-cols-2 text-center border-b border-white/5 last:border-0 hover:bg-white/5 transition-colors">
                <div className="py-1.5 flex justify-between px-2">
                  <span className="text-slate-500">{bid.qty > 0 ? bid.qty : "-"}</span>
                  <span className={bid.price > 0 ? "text-success" : "text-slate-600"}>{bid.price > 0 ? bid.price.toFixed(2) : "-"}</span>
                </div>
                <div className="py-1.5 border-l border-white/5 flex justify-between px-2">
                  <span className={ask.price > 0 ? "text-danger" : "text-slate-600"}>{ask.price > 0 ? ask.price.toFixed(2) : "-"}</span>
                  <span className="text-slate-500">{ask.qty > 0 ? ask.qty : "-"}</span>
                </div>
              </div>
            );
          })}
        </div>
      </CardContent>
    </Card>
  );
}
