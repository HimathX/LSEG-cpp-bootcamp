from __future__ import annotations

import argparse
import csv
import random
from pathlib import Path


SEED = 20260326
DEFAULT_ROWS = 1500
ROWS_PER_CYCLE = 50
HEADER = ["Client Order ID", "Instrument", "Side", "Quantity", "Price"]
VALID_INSTRUMENTS = ["Rose", "Lavender", "Lotus", "Tulip", "Orchid"]
BASE_PRICES = {
    "Rose": 42.00,
    "Lavender": 24.00,
    "Lotus": 30.00,
    "Tulip": 35.00,
    "Orchid": 45.00,
}


def format_value(value: object) -> str:
    if value is None:
        return ""
    if isinstance(value, float):
        return f"{value:.2f}"
    return str(value)


class SampleBuilder:
    def __init__(self) -> None:
        self.rows: list[list[str]] = []
        self.client_counter = 1

    def next_client_id(self) -> str:
        client_id = f"S8{self.client_counter:04d}"
        self.client_counter += 1
        return client_id

    def add_row(self, *fields: object) -> None:
        self.rows.append([format_value(field) for field in fields])

    def add_valid(
        self,
        instrument: str,
        side: int,
        quantity: int,
        price: float,
        client_id: str | None = None,
    ) -> None:
        self.add_row(client_id or self.next_client_id(), instrument, side, quantity, price)

    def add_invalid_side(self, instrument: str, price: float, side: int = 9) -> None:
        self.add_row(self.next_client_id(), instrument, side, 50, price)

    def add_invalid_size(self, instrument: str, price: float, quantity: object = 25) -> None:
        self.add_row(self.next_client_id(), instrument, 1, quantity, price)

    def add_invalid_instrument(self, price: float) -> None:
        self.add_row(self.next_client_id(), "Daisy", 1, 100, price)

    def add_extra_field(self, instrument: str, price: float) -> None:
        self.add_row(self.next_client_id(), instrument, 1, 100, price, "EXTRA")

    def add_invalid_price(self, instrument: str, price: float = 0.00) -> None:
        self.add_row(self.next_client_id(), instrument, 1, 100, price)

    def add_parse_reject(self, variant: str, instrument: str, price: float) -> None:
        if variant == "blank_client":
            self.add_row("", instrument, 1, 100, price)
        elif variant == "blank_instrument":
            self.add_row(self.next_client_id(), "", 2, 100, price)
        elif variant == "blank_side":
            self.add_row(self.next_client_id(), instrument, "", 100, price)
        elif variant == "nonnumeric_qty":
            self.add_row(self.next_client_id(), instrument, 1, "OneHundred", price)
        elif variant == "nonnumeric_price":
            self.add_row(self.next_client_id(), instrument, 2, 100, "Free")
        elif variant == "blank_price":
            self.add_row(self.next_client_id(), instrument, 2, 100, "")
        else:
            raise ValueError(f"Unknown parse reject variant: {variant}")


def price_for(instrument: str, cycle: int, slot: int) -> float:
    base_price = BASE_PRICES[instrument]
    increment = 0.25 * ((cycle + slot) % 8)
    return round(base_price + increment, 2)


def pick(rng: random.Random, values: list[int]) -> int:
    return values[rng.randrange(len(values))]


def append_cycle(builder: SampleBuilder, rng: random.Random, cycle: int) -> None:
    instruments = VALID_INSTRUMENTS
    inst_a = instruments[cycle % len(instruments)]
    inst_b = instruments[(cycle + 1) % len(instruments)]
    inst_c = instruments[(cycle + 2) % len(instruments)]
    inst_d = instruments[(cycle + 3) % len(instruments)]
    inst_e = instruments[(cycle + 4) % len(instruments)]

    p_a = price_for(inst_a, cycle, 0)
    p_b = price_for(inst_b, cycle, 1)
    p_c = price_for(inst_c, cycle, 2)
    p_d = price_for(inst_d, cycle, 3)
    p_e = price_for(inst_e, cycle, 4)

    q_a1 = pick(rng, [40, 50, 60, 70])
    q_a2 = pick(rng, [60, 70, 80, 90])
    q_b0 = pick(rng, [20, 30, 40, 50, 60])
    q_b_sweep_top = pick(rng, [50, 60, 70])
    q_b_sweep_mid = pick(rng, [60, 70, 80])
    q_b_sweep_low = pick(rng, [80, 90, 100])
    q_c_initial_sell = pick(rng, [80, 90, 100])
    q_c_aggressive_buy = q_c_initial_sell + pick(rng, [30, 40, 50])
    q_d_base_buy = pick(rng, [60, 70, 80])
    q_e_passive_sell = pick(rng, [80, 90, 100])
    q_e_aggressive_buy = q_e_passive_sell + pick(rng, [20, 30, 40])

    parse_variants = [
        "blank_client",
        "blank_instrument",
        "blank_side",
        "nonnumeric_qty",
        "nonnumeric_price",
        "blank_price",
    ]
    parse_variant_one = parse_variants[cycle % len(parse_variants)]
    parse_variant_two = parse_variants[(cycle + 3) % len(parse_variants)]

    # Scenario 1: same-price FIFO on instrument A with later cleanup
    builder.add_valid(inst_a, 1, q_a1, p_a)
    builder.add_valid(inst_a, 1, q_a2, p_a)
    builder.add_invalid_side(inst_a, p_a)
    builder.add_valid(inst_a, 2, q_a1 + 20, round(p_a - 0.25, 2))
    builder.add_valid(inst_a, 2, q_a2 - 20, p_a)
    builder.add_valid(inst_a, 2, 30, round(p_a + 1.00, 2))
    builder.add_valid(inst_a, 1, 10, round(p_a + 1.00, 2))
    builder.add_valid(inst_a, 1, 20, round(p_a + 1.25, 2))
    builder.add_valid(inst_b, 1, q_b0, round(p_b - 0.50, 2))
    builder.add_valid(inst_b, 2, q_b0, round(p_b - 0.50, 2))

    # Scenario 2: multi-level sweep and aggressive remainder on instrument B
    builder.add_valid(inst_b, 1, q_b_sweep_top, round(p_b + 1.00, 2))
    builder.add_invalid_size(inst_b, round(p_b + 0.50, 2))
    builder.add_valid(inst_b, 1, q_b_sweep_mid, round(p_b + 0.50, 2))
    builder.add_valid(inst_b, 1, q_b_sweep_low, p_b)
    builder.add_valid(inst_b, 2, q_b_sweep_top + q_b_sweep_mid + 60, p_b)
    builder.add_valid(inst_b, 2, q_b_sweep_low - 60, p_b)
    builder.add_invalid_instrument(round(p_b + 3.00, 2))
    builder.add_valid(inst_b, 2, 40, round(p_b + 1.25, 2))
    builder.add_valid(inst_b, 1, 60, round(p_b + 1.25, 2))
    builder.add_valid(inst_b, 2, 20, round(p_b + 1.00, 2))

    # Scenario 3: aggressive remainder rests, then later cleanup on instrument C
    builder.add_valid(inst_c, 2, q_c_initial_sell, p_c)
    builder.add_valid(inst_c, 1, q_c_aggressive_buy, round(p_c + 0.75, 2))
    builder.add_parse_reject(parse_variant_one, inst_c, round(p_c + 0.25, 2))
    builder.add_valid(inst_c, 2, 20, round(p_c + 0.25, 2))
    builder.add_valid(inst_c, 2, q_c_aggressive_buy - q_c_initial_sell - 20, round(p_c + 0.75, 2))
    builder.add_valid(inst_c, 2, 50, round(p_c + 1.50, 2))
    builder.add_valid(inst_c, 1, 30, round(p_c + 1.50, 2))
    builder.add_extra_field(inst_c, round(p_c + 1.25, 2))
    builder.add_valid(inst_c, 1, 20, round(p_c + 1.75, 2))
    builder.add_valid(inst_d, 1, q_d_base_buy, p_d)

    # Scenario 4: non-crossing new orders inserted mid-stream on instrument D
    builder.add_valid(inst_d, 1, 100, p_d)
    builder.add_valid(inst_d, 2, 100, round(p_d + 1.50, 2))
    builder.add_parse_reject(parse_variant_two, inst_e, round(p_e + 0.50, 2))
    builder.add_valid(inst_d, 1, 40, round(p_d + 0.50, 2))
    builder.add_valid(inst_d, 2, 70, round(p_d + 0.50, 2))
    builder.add_valid(inst_d, 1, 30, round(p_d + 1.00, 2))
    builder.add_invalid_price(inst_d, 0.00)
    builder.add_valid(inst_d, 1, 60, p_d)
    builder.add_valid(inst_d, 2, 60, p_d)
    builder.add_valid(inst_e, 2, q_e_passive_sell, p_e)

    # Scenario 5: same instrument cleanup plus more interleaved invalids on instrument E
    builder.add_valid(inst_e, 1, q_e_aggressive_buy, round(p_e + 0.50, 2))
    builder.add_parse_reject("blank_client", inst_e, round(p_e + 0.25, 2))
    builder.add_valid(inst_e, 2, 10, round(p_e + 0.25, 2))
    builder.add_valid(inst_e, 2, q_e_aggressive_buy - q_e_passive_sell - 10, round(p_e + 0.50, 2))
    builder.add_valid(inst_e, 1, 90, round(p_e - 0.50, 2))
    builder.add_valid(inst_e, 2, 100, round(p_e - 0.75, 2))
    builder.add_invalid_side(inst_e, round(p_e - 0.50, 2), side=0)
    builder.add_valid(inst_e, 1, 10, round(p_e - 0.50, 2))
    builder.add_valid(inst_a, 1, 50, round(p_a - 1.00, 2))
    builder.add_valid(inst_a, 2, 50, round(p_a - 1.00, 2))


def build_sample(row_count: int) -> list[list[str]]:
    if row_count % ROWS_PER_CYCLE != 0:
        raise ValueError(f"row_count must be a multiple of {ROWS_PER_CYCLE}")

    rng = random.Random(SEED)
    builder = SampleBuilder()
    cycles = row_count // ROWS_PER_CYCLE

    for cycle in range(cycles):
        append_cycle(builder, rng, cycle)

    if len(builder.rows) != row_count:
        raise AssertionError(f"Expected {row_count} rows, generated {len(builder.rows)} rows")

    return builder.rows


def write_sample(rows: list[list[str]], output_path: Path) -> None:
    output_path.parent.mkdir(parents=True, exist_ok=True)
    with output_path.open("w", newline="", encoding="utf-8") as file:
        writer = csv.writer(file)
        writer.writerow(HEADER)
        writer.writerows(rows)


def parse_args() -> argparse.Namespace:
    repo_root = Path(__file__).resolve().parents[1]
    default_output = repo_root / "tests" / "sample_orders_8.csv"

    parser = argparse.ArgumentParser(description="Generate a deterministic large sample_orders_8.csv dataset.")
    parser.add_argument("--rows", type=int, default=DEFAULT_ROWS, help="Number of data rows to generate.")
    parser.add_argument("--output", type=Path, default=default_output, help="Output CSV path.")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    rows = build_sample(args.rows)
    write_sample(rows, args.output)
    print(f"Wrote {len(rows)} rows to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
