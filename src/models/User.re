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
      ~showLabel=v => v,
      Person.Name,
    ),
    makeOrdered(
      (module Int.Ord),
      ~title="Age",
      ~showLabel=Int.Show.show,
      Person.Age,
    ),
    makeUnordered(
      ~title="Last Login",
      ~showLabel=Option.maybe(~default="-", ~f=Js.Date.toDateString),
      Person.LastLogin,
    ),
  ];

[@react.component]
let make = () => <Table makeKey={v => v.Person.id} data=rows columns />;
