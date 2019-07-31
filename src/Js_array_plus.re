open Js_array;

let init = (length, fn) => {
  let rec _make = (l: int, v: int => 'a, a: array('a)): array('a) =>
    switch (l) {
    | 0 => a
    | l_ => _make(l_ - 1, v, concat(a, [|v(l_ - 1)|]))
    };

  _make(length, fn, [||]);
};

let make = (length, value) => init(length, _ => value);

let init_matrix = (dimx, dimy, fn) =>
  init(dimy, y => init(dimx, x => fn((x, y))));

let make_matrix = (dimx, dimy, value) => init_matrix(dimx, dimy, _ => value);

let pad = (v, padding, arr) =>
  make(padding, v) |> concat(arr) |> concat(make(padding, v));

let pad_matrix = (v, padding, matrix) =>
  pad(
    make(length(unsafe_get(matrix, 0)) + padding * 2, v),
    padding,
    map(pad(v, padding), matrix),
  );

let transpose = matrix => {
  let dimy = length(matrix);
  let dimx = length(unsafe_get(matrix, 0));

  init(dimx, x => init(dimy, y => unsafe_get(unsafe_get(matrix, y), x)));
};

let trimi = (predicate, arr) => {
  let start = findIndexi((item, i) => !predicate(item, i), arr);
  let end_ =
    arr
    |> Belt.Array.reverse
    |> findIndexi((item, i) => !predicate(item, i))
    |> (e => e === 0 ? length(arr) : - e);

  slice(~start, ~end_, arr);
};

let trim = predicate => trimi((item, _) => predicate(item));

let trim_matrix = (predicate, arr) =>
  trim(every(predicate), arr)
  |> transpose
  |> trim(every(predicate))
  |> transpose;
