open React;
open Js_array;
open Js_array_plus;
open ReactDOMRe;
open Game_model;

let figure_size = figure => {
  let maxX = reduce((max, (x, _)) => max > x ? max : x, 0, figure);
  let maxY = reduce((max, (_, y)) => max > y ? max : y, 0, figure);

  (maxX + 1, maxY + 1);
};

let figure_to_matrix = figure => {
  let (dimx, dimy) = figure_size(figure);
  init_matrix(dimx, dimy, point =>
    some(figure_point => figure_point == point, figure) ? Alive : Dead
  );
};

let evolve = figure =>
  figure
  |> pad_matrix(Dead, 1)
  |> next_generation
  |> trim_matrix(s => s == Dead);

let figure_states = figure => {
  let rec _make = (states, tries) => {
    let next = states |> unsafe_get(_, 0) |> evolve;
    let exists = some(s => s == next, states);
    let new_states = exists ? states : concat(states, [|next|]);

    if (tries <= 0 || exists) {
      new_states;
    } else {
      _make(new_states, tries - 1);
    };
  };

  _make([|figure|], 5);
};

[@react.component]
let make = (~coords) => {
  let all_states = coords |> figure_to_matrix |> figure_states;
  let (current, setCurrent) = useState(() => 0);

  useEffect(() =>
    Js_global.setInterval(
      () => setCurrent(n => n + 1 === length(all_states) ? 0 : n + 1),
      1000,
    )
    |> (id => Some(() => Js_global.clearInterval(id)))
  );

  <Matrix
    value={unsafe_get(all_states, current)}
    cellSize="10px"
    style={(_, state) =>
      Style.make(
        ~background=
          switch (state) {
          | Alive => "gray"
          | Dead => "#fff"
          },
        (),
      )
    }
  />;
};
