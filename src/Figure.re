[@bs.config {jsx: 3}];
open Js_array;
open Js_array_plus;
open ReactDOMRe;
open Game_model;

let figureSize = figure => {
  let maxX = reduce((max, (x, _)) => max > x ? max : x, 0, figure);
  let maxY = reduce((max, (_, y)) => max > y ? max : y, 0, figure);

  (maxX + 1, maxY + 1);
};

let figureToMatrix = figure => {
  let (dimx, dimy) = figureSize(figure);
  init_matrix(dimx, dimy, point =>
    some(figurePoint => figurePoint == point, figure) ? Alive : Dead
  );
};

[@react.component]
let make = (~coords) => {
  <Matrix
    value={figureToMatrix(coords)}
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
