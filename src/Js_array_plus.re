open Js_array;

let init_matrix = (dimx, dimy, fn) =>
  Belt_Array.makeBy(dimy, y => Belt_Array.makeBy(dimx, x => fn((x, y))));

let make_matrix = (dimx, dimy, value) => init_matrix(dimx, dimy, _ => value);

let pad = (v, padding, arr) =>
  Belt_Array.make(padding, v)
  |> concat(arr)
  |> concat(Belt_Array.make(padding, v));

let pad_matrix = (v, padding, matrix) =>
  pad(
    Belt_Array.make(length(unsafe_get(matrix, 0)) + padding * 2, v),
    padding,
    map(pad(v, padding), matrix),
  );

let transpose = matrix => {
  let dimy = length(matrix);
  let dimx = length(unsafe_get(matrix, 0));

  Belt_Array.(
    makeBy(dimx, x =>
      makeBy(dimy, y => unsafe_get(unsafe_get(matrix, y), x))
    )
  );
};

let trimi = (predicate, arr) => {
  let start = findIndexi((item, i) => !predicate(item, i), arr);
  let end_ =
    arr
    |> Belt_Array.reverse
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
