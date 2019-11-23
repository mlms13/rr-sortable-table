open BsAbstract;
open User;

module Table = SortableTable.Make(Person);

let rows = User.sample;

let columns =
  Table.Column.[
    makeOrdered(
      (module String.Ord),
      ~title="Name",
      ~render=React.string,
      Person.Name,
    ),
    makeOrdered(
      (module Option.Ord(String.Ord)),
      ~title="Job",
      ~render=Option.maybe(~default=React.string("-"), ~f=React.string),
      Person.Job,
    ),
    makeOrdered(
      (module Int.Ord),
      ~title="Apartment",
      ~render=v => React.string(Int.Show.show(v)),
      Person.Apartment,
    ),
    makeOrdered(
      (module JsDate.Ord),
      ~title="Birthday",
      ~render=v => React.string(Js.Date.toDateString(v)),
      Person.Birthday,
    ),
  ];

[@react.component]
let make = () => <Table makeRowKey={v => v.Person.id} rows columns />;
