[@bs.config {jsx: 3}];
open ReasonReact;
open Js_array;

[@react.component]
let make = (~figures) => {
  <ul
    style={ReactDOMRe.Style.make(
      ~display="flex",
      ~padding="0",
      ~margin="0",
      (),
    )}>
    {array(
       map(
         ((name, coords)) =>
           <li
             key=name
             style={ReactDOMRe.Style.make(
               ~display="flex",
               ~flexDirection="column",
               ~alignItems="center",
               ~justifyContent="space-between",
               ~padding="5px 10px",
               (),
             )}>
             <Figure coords />
             <span> {ReasonReact.string(name)} </span>
           </li>,
         figures,
       ),
     )}
  </ul>;
};

module Jsx2 = {
  let component = statelessComponent("FigurePreviews");
  let make = (~figures) =>
    ReasonReactCompat.wrapReactForReasonReact(make, makeProps(~figures, ()));
};
