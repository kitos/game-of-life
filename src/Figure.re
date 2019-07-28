[@bs.config {jsx: 3}];
open Js_array;
open Js_array_plus;

let figureSize = figure => {
  let maxX = reduce((max, (x, _)) => max > x ? max : x, 0, figure);
  let maxY = reduce((max, (_, y)) => max > y ? max : y, 0, figure);

  (maxX + 1, maxY + 1);
};

let figureToMatrix = figure => {
  let (dimx, dimy) = figureSize(figure);
  init_matrix(dimx, dimy, point =>
    some(figurePoint => figurePoint == point, figure)
  );
};

let tableStyle =
  ReactDOMRe.Style.make(
    ~display="inline-block",
    ~border="0 solid lightgray",
    ~borderTopWidth="1px",
    ~borderLeftWidth="1px",
    (),
  );

let cellSize = "10px";

let cellStyle =
  ReactDOMRe.Style.make(
    ~height=cellSize,
    ~width=cellSize,
    ~border="0 solid lightgray",
    ~borderRightWidth="1px",
    ~borderBottomWidth="1px",
    (),
  );
let figureCellStyle = ReactDOMRe.Style.make(~background="gray", ());

[@react.component]
let make = (~coords) => {
  <div style=tableStyle>
    {ReasonReact.array(
       mapi(
         (row, y) =>
           <div
             key={string_of_int(y)}
             style={ReactDOMRe.Style.make(~display="flex", ())}>
             {ReasonReact.array(
                mapi(
                  (isFigureCell, x) =>
                    <div
                      key={string_of_int(x)}
                      className="figure"
                      style={
                        isFigureCell
                          ? ReactDOMRe.Style.combine(
                              cellStyle,
                              figureCellStyle,
                            )
                          : cellStyle
                      }
                    />,
                  row,
                ),
              )}
           </div>,
         figureToMatrix(coords),
       ),
     )}
  </div>;
};

module Jsx2 = {
  let component = ReasonReact.statelessComponent("Figure");
  let make = (~coords) =>
    ReasonReactCompat.wrapReactForReasonReact(make, makeProps(~coords, ()));
};
