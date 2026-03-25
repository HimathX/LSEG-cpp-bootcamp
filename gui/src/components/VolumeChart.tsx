import { Card, CardHeader, CardContent } from "@/components/ui/card";
import { BarChart, Bar, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer, Cell } from "recharts";

interface VolumeData {
  name: string;
  volume: number;
}

interface VolumeChartProps {
  data: VolumeData[];
  theme: "light" | "dark";
}

export function VolumeChart({ data, theme }: VolumeChartProps) {
  // Use a nice array of colors for the bars to match the dark neon aesthetic
  const colors = ["#3b82f6", "#8b5cf6", "#ec4899", "#f59e0b", "#10b981"];
  const isDark = theme === "dark";
  const gridStroke = isDark ? "#ffffff1a" : "#0f172a1a";
  const tickColor = isDark ? "#94a3b8" : "#475569";
  const tooltipBackground = isDark ? "#09090b" : "#ffffff";
  const tooltipBorder = isDark ? "1px solid #ffffff1a" : "1px solid #cbd5e1";
  const tooltipLabel = isDark ? "#e2e8f0" : "#0f172a";

  return (
    <Card className="border-border bg-card w-full h-[300px]">
      <CardHeader className="py-3 px-4 flex justify-between items-center bg-muted/50 border-b border-border">
        <h3 className="text-sm font-bold tracking-wide text-foreground uppercase">
          Market Volume
        </h3>
      </CardHeader>
      <CardContent className="p-4">
        <div className="w-full h-full min-h-[200px]">
          <ResponsiveContainer width="100%" height="100%">
            <BarChart
              data={data}
              margin={{ top: 10, right: 10, left: -20, bottom: 0 }}
            >
              <CartesianGrid strokeDasharray="3 3" stroke={gridStroke} vertical={false} />
              <XAxis
                dataKey="name"
                tick={{ fill: tickColor, fontSize: 12 }}
                axisLine={{ stroke: gridStroke }}
              />
              <YAxis
                tick={{ fill: tickColor, fontSize: 12 }}
                axisLine={{ stroke: gridStroke }}
                tickFormatter={(val) => `${val}`}
              />
              <Tooltip
                cursor={{ fill: isDark ? "#ffffff0a" : "#0f172a0d" }}
                contentStyle={{ backgroundColor: tooltipBackground, border: tooltipBorder, borderRadius: "8px" }}
                labelStyle={{ color: tooltipLabel, fontWeight: "bold", marginBottom: "4px" }}
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
      </CardContent>
    </Card>
  );
}
