import { Card, CardHeader, CardContent as CardBody } from "@heroui/react";
import { BarChart, Bar, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer, Cell } from "recharts";

interface VolumeData {
  name: string;
  volume: number;
}

interface VolumeChartProps {
  data: VolumeData[];
}

export function VolumeChart({ data }: VolumeChartProps) {
  // Use a nice array of colors for the bars to match the dark neon aesthetic
  const colors = ["#3b82f6", "#8b5cf6", "#ec4899", "#f59e0b", "#10b981"];

  return (
    <Card className="bg-zinc-900 border border-white/10 w-full h-[300px]">
      <CardHeader className="py-3 px-4 flex justify-between items-center bg-black/20 border-b border-white/5">
        <h3 className="text-sm font-bold tracking-wide text-slate-300 uppercase">
          Market Volume
        </h3>
      </CardHeader>
      <CardBody className="p-4">
        <div className="w-full h-full min-h-[200px]">
          <ResponsiveContainer width="100%" height="100%">
            <BarChart
              data={data}
              margin={{ top: 10, right: 10, left: -20, bottom: 0 }}
            >
              <CartesianGrid strokeDasharray="3 3" stroke="#ffffff1a" vertical={false} />
              <XAxis 
                dataKey="name" 
                tick={{ fill: "#94a3b8", fontSize: 12 }} 
                axisLine={{ stroke: "#ffffff1a" }}
              />
              <YAxis 
                tick={{ fill: "#94a3b8", fontSize: 12 }} 
                axisLine={{ stroke: "#ffffff1a" }}
                tickFormatter={(val) => `${val}`}
              />
              <Tooltip 
                cursor={{ fill: "#ffffff0a" }}
                contentStyle={{ backgroundColor: "#09090b", border: "1px solid #ffffff1a", borderRadius: "8px" }}
                labelStyle={{ color: "#e2e8f0", fontWeight: "bold", marginBottom: "4px" }}
                itemStyle={{ color: "#10b981", fontWeight: "600" }}
              />
              <Bar dataKey="volume" radius={[4, 4, 0, 0]}>
                {data.map((_, index) => (
                  <Cell key={`cell-${index}`} fill={colors[index % colors.length]} />
                ))}
              </Bar>
            </BarChart>
          </ResponsiveContainer>
        </div>
      </CardBody>
    </Card>
  );
}
