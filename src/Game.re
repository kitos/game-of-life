open React;
open Game_model;

type state = {
  inProgress: bool,
  fieldSize: int,
  interval: int,
  cells: array(array(cellState)),
  generation: int,
};

type action =
  | Init
  | Clear
  | ToggleGame(bool)
  | ToggleCell((int, int))
  | Evolve
  | SetFieldSize(int)
  | SetInterval(int);

let colorByState = (state: cellState) =>
  switch (state) {
  | Alive => "#357720"
  | Dead => "#bdbdbd"
  };

let reducer = (state, action) =>
  switch (action) {
  | Init => {
      ...state,
      inProgress: false,
      generation: 0,
      cells:
        Js_array_plus.init_matrix(state.fieldSize, state.fieldSize, _ =>
          Js_math.random() > 0.7 ? Alive : Dead
        ),
    }
  | ToggleGame(inProgress) => {...state, inProgress}
  | Clear => {
      ...state,
      inProgress: false,
      generation: 0,
      cells:
        Js_array_plus.make_matrix(state.fieldSize, state.fieldSize, Dead),
    }
  | ToggleCell(coord) => {
      ...state,
      generation: 0,
      cells: toggle(state.cells, coord),
    }
  | Evolve => {
      ...state,
      cells: next_generation(state.cells),
      generation: state.generation + 1,
    }
  | SetFieldSize(value) => {...state, fieldSize: Js_math.max_int(5, value)}
  | SetInterval(value) => {...state, interval: Js_math.max_int(50, value)}
  };

let useGame = () => {
  let (state, dispatch) =
    useReducerWithMapState(
      reducer,
      {
        inProgress: false,
        fieldSize: 25,
        interval: 300,
        cells: [|[||]|],
        generation: 0,
      },
      reducer(_, Init),
    );
  let {inProgress, interval} = state;

  useEffect2(
    () =>
      if (inProgress) {
        Js_global.(
          setInterval(() => dispatch(Evolve), interval)
          |> (tid => Some(() => clearInterval(tid)))
        );
      } else {
        None;
      },
    (inProgress, interval),
  );

  (state, dispatch);
};

[@react.component]
let make = () => {
  let ({inProgress, fieldSize, interval, generation, cells}, dispatch) =
    useGame();

  <div>
    <h2> {ReasonReact.string({j|Generation: $generation|j})} </h2>
    <Matrix
      value=cells
      style={(_, state) =>
        ReactDOMRe.Style.make(~background=colorByState(state), ())
      }
      onCellClick={(_, coords, _) => dispatch(ToggleCell(coords))}
    />
    <p>
      <button
        onClick={_ => {
          dispatch(ToggleGame(false));
          dispatch(Evolve);
        }}>
        {ReasonReact.string("Evolve")}
      </button>
      <button onClick={_ => dispatch(ToggleGame(!inProgress))}>
        {ReasonReact.string(inProgress ? "Pause" : "Start")}
      </button>
      <button onClick={_ => {dispatch(Init)}}>
        {ReasonReact.string("Random")}
      </button>
      <button onClick={_ => dispatch(Clear)}>
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
          value={string_of_int(fieldSize)}
          onChange={_event => {
            dispatch(SetFieldSize(ReactEvent.Form.target(_event)##value));
            dispatch(Init);
          }}
        />
      </label>
      <label>
        {ReasonReact.string("Interval: ")}
        <input
          type_="number"
          min=50
          max="5000"
          value={string_of_int(interval)}
          onChange={_event =>
            dispatch(SetInterval(ReactEvent.Form.target(_event)##value))
          }
        />
      </label>
    </p>
  </div>;
};
