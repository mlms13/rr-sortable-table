open BsAbstract;

module Person = [%lenses
  type data = {
    id: string,
    name: string,
    age: int,
    lastLogin: option(Js.Date.t),
  }
];

module Table = Table.Make(Person);

let rows =
  Person.[
    {
      id: "abc",
      name: "Bill",
      age: 38,
      lastLogin:
        Some(Js.Date.makeWithYMD(~year=2019., ~month=3., ~date=12., ())),
    },
    {id: "def", name: "Sue", age: 31, lastLogin: None},
    {
      id: "ghi",
      name: "Terry",
      age: 28,
      lastLogin:
        Some(Js.Date.makeWithYMD(~year=2018., ~month=9., ~date=1., ())),
    },
  ];

let columns =
  Table.Column.[
    makeOrdered(
      (module String.Ord),
      ~title="Name",
      ~render=React.string,
      Person.Name,
    ),
    makeOrdered(
      (module Int.Ord),
      ~title="Age",
      ~render=v => React.string(Int.Show.show(v)),
      Person.Age,
    ),
    makeOrdered(
      (module Option.Ord(JsDate.Ord)),
      ~title="Last Login",
      ~render=
        v =>
          Option.maybe(~default="-", ~f=Js.Date.toDateString, v)
          |> React.string,
      Person.LastLogin,
    ),
  ];

[@react.component]
let make = () => <Table makeRowKey={v => v.Person.id} data=rows columns />;
