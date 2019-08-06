open React;

[@react.component]
let make = () => {
  <div>
    <h1> {string("Game of life")} </h1>
    <p>
      <a
        href="https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life"
        target="_blank">
        {string("What is it? (Wiki)")}
      </a>
    </p>
    <h2> {string("Figures")} </h2>
    <FigurePreviews
      figures=[|
        ("Glider", [|(1, 0), (2, 1), (0, 2), (1, 2), (2, 2)|]),
        ("Blinker", [|(0, 0), (1, 0), (2, 0)|]),
        ("Beacon", [|(0, 0), (1, 0), (0, 1), (3, 3), (3, 2), (2, 3)|]),
      |]
    />
    <Game.Jsx3 dummy=true />
  </div>;
};
