open Game_model;

type state = {
  fieldSize: int,
  interval: int,
  cells: array(array(cellState)),
  generation: int,
  intervalId: option(Js_global.intervalId),
};

type action =
  | Init
  | Clear
  | Toggle((int, int))
  | Evolve
  | Start
  | Stop
  | SetIntervalId(option(Js_global.intervalId))
  | SetFieldSize(int)
  | SetInterval(int);

let classByState = (state: cellState) =>
  switch (state) {
  | Alive => "alive"
  | Dead => "dead"
  };

let initialFieldSize = 25;

let component = ReasonReact.reducerComponent("Grid");

let make = _ => {
  ...component,

  initialState: () => {
    fieldSize: initialFieldSize,
    interval: 300,
    cells: [|[||]|],
    generation: 0,
    intervalId: None,
  },

  reducer: (action, state) =>
    switch (action) {
    | Init =>
      ReasonReact.Update({
        ...state,
        generation: 0,
        cells:
          Js_array_plus.init_matrix(state.fieldSize, state.fieldSize, () =>
            Js_math.random() > 0.7 ? Alive : Dead
          ),
      })
    | Clear =>
      ReasonReact.Update({
        ...state,
        generation: 0,
        cells:
          Js_array_plus.make_matrix(state.fieldSize, state.fieldSize, Dead),
      })
    | Toggle(coord) =>
      ReasonReact.Update({
        ...state,
        generation: 0,
        cells: toggle(state.cells, coord),
      })
    | Evolve =>
      ReasonReact.Update({
        ...state,
        cells: next_generation(state.cells),
        generation: state.generation + 1,
      })
    | Start =>
      ReasonReact.SideEffects(
        ({send}) =>
          switch (state.intervalId) {
          | None =>
            let id =
              Js_global.setInterval(() => send(Evolve), state.interval);
            send(SetIntervalId(Some(id)));
          | Some(_) => Js_console.log("Game has been already started.")
          },
      )
    | Stop =>
      ReasonReact.SideEffects(
        ({send, state}) =>
          switch (state.intervalId) {
          | Some(id) =>
            Js_global.clearInterval(id);
            send(SetIntervalId(None));
          | None =>
            Js_console.log(
              "Game has not been started yet or has be stoped already.",
            )
          },
      )
    | SetIntervalId(value) =>
      ReasonReact.Update({...state, intervalId: value})
    | SetFieldSize(value) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, fieldSize: Js_math.max_int(5, value)},
        ({send}) => send(Init),
      )
    | SetInterval(value) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, interval: Js_math.max_int(50, value)},
        ({send}) => send(Stop),
      )
    },

  didMount: ({send, onUnmount}) => {
    send(Init);
    onUnmount(() => send(Stop));
  },

  render: ({state, send}) => {
    let inProgress = state.intervalId !== None;
    let generation = state.generation;

    <div>
      <h1> {ReasonReact.string("Game of life")} </h1>
      <p>
        <a
          href="https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life"
          target="_blank">
          {ReasonReact.string("What is it? (Wiki)")}
        </a>
      </p>
      <h2> {ReasonReact.string({j|Generation: $generation|j})} </h2>
      {ReasonReact.array(
         Js_array.mapi(
           (row, y) =>
             <div key={string_of_int(y)} className="row">
               {ReasonReact.array(
                  Js_array.mapi(
                    (cs, x) =>
                      <div
                        key={string_of_int(x)}
                        className={"cell " ++ classByState(cs)}
                        onClick={_ => send(Toggle((x, y)))}
                      />,
                    row,
                  ),
                )}
             </div>,
           state.cells,
         ),
       )}
      <p>
        <button
          onClick={_ => {
            send(Stop);
            send(Evolve);
          }}>
          {ReasonReact.string("Evolve")}
        </button>
        <button onClick={_ => send(inProgress ? Stop : Start)}>
          {ReasonReact.string(inProgress ? "Pause" : "Start")}
        </button>
        <button
          onClick={_ => {
            send(Stop);
            send(Init);
          }}>
          {ReasonReact.string("Random")}
        </button>
        <button
          onClick={_ => {
            send(Stop);
            send(Clear);
          }}>
          {ReasonReact.string("Clear")}
        </button>
      </p>
      <p>
        <label>
          {ReasonReact.string("Field size: ")}
          <input
            type_="number"
            min=5
            max="50"
            value={string_of_int(state.fieldSize)}
            onChange={_event =>
              send(SetFieldSize(ReactEvent.Form.target(_event)##value))
            }
          />
        </label>
        <label>
          {ReasonReact.string("Interval: ")}
          <input
            type_="number"
            min=50
            max="5000"
            value={string_of_int(state.interval)}
            onChange={_event =>
              send(SetInterval(ReactEvent.Form.target(_event)##value))
            }
          />
        </label>
      </p>
    </div>;
  },
};
