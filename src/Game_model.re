type cellState =
  | Dead
  | Alive;

let neighbours_deltas = [((-1), (-1)), (0, (-1)), (1, (-1)), (1, 0), (1, 1), (0, 1), ((-1), 1), ((-1), 0)];

let fix_outbound = (arr, i) =>
  if (i < 0) {
    Js_array.length(arr) - 1;
  } else if (i >= Js_array.length(arr)) {
    0;
  } else {
    i;
  };

let fix_outbound2 = (cells, (x, y)) => {
  let fixedX = fix_outbound(cells[0], x);
  let fixedY = fix_outbound(cells, y);

  (fixedX, fixedY);
};

let count_neighbours = (state: cellState, cells, (x, y)) =>
  neighbours_deltas
  |> List.map(((dx, dy)) => (x + dx, y + dy))
  |> List.map(fix_outbound2(cells))
  |> List_plus.count(((x, y)) => cells[y][x] === state);

let count_alive_neighbours = count_neighbours(Alive);

let toggle = (cells, (x, y)) => {
  switch (cells[y][x]) {
  | Alive => cells[y][x] = Dead
  | Dead => cells[y][x] = Alive
  };
  cells;
};

let next_generation = cells =>
  Js_array.mapi(
    (row, y) =>
      Js_array.mapi(
        (cell, x) =>
          switch (cell, count_alive_neighbours(cells, (x, y))) {
          | (Alive, 2 | 3)
          | (Dead, 3) => Alive
          | _ => Dead
          },
        row,
      ),
    cells,
  );
