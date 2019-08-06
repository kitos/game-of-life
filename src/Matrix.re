open ReactDOMRe;

let cellStyle = cellSize =>
  Style.make(
    ~height=cellSize,
    ~width=cellSize,
    ~border="0 solid lightgray",
    ~borderRightWidth="1px",
    ~borderBottomWidth="1px",
    (),
  );

let matrixStyle =
  ReactDOMRe.Style.make(
    ~display="inline-block",
    ~border="0 solid lightgray",
    ~borderTopWidth="1px",
    ~borderLeftWidth="1px",
    (),
  );

[@react.component]
let make =
    (
      ~value,
      ~cellSize="15px",
      ~style=(_, _) => Style.make(),
      ~onCellClick=(_, _, _) => (),
    ) => {
  <div style=matrixStyle>
    {ReasonReact.array(
       Js_array.mapi(
         (row, y) =>
           <div
             key={string_of_int(y)} style={Style.make(~display="flex", ())}>
             {ReasonReact.array(
                Js_array.mapi(
                  (el, x) =>
                    <div
                      key={string_of_int(x)}
                      style={Style.combine(
                        cellStyle(cellSize),
                        style((x, y), el),
                      )}
                      className="Matrix__cell"
                      onClick={e => onCellClick(e, (x, y), el)}
                    />,
                  row,
                ),
              )}
           </div>,
         value,
       ),
     )}
  </div>;
};

module Jsx2 = {
  let component = ReasonReact.statelessComponent("Matrix");
  let make =
      (~value, ~cellSize="15px", ~style=(_, _) => Style.make(), ~onCellClick) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~value, ~cellSize, ~style, ~onCellClick, ()),
    );
};
