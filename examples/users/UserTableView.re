open BsAbstract;
open User;

let rows = User.sample;

let columns = [
  SortableTable.orderedColumn(
    (module String.Ord),
    ~title="Name",
    ~render=React.string,
    Person.get(_, Name),
  ),
  SortableTable.orderedColumn(
    (module Option.Ord(String.Ord)),
    ~title="Job",
    ~render=Option.maybe(~default=React.string("-"), ~f=React.string),
    Person.get(_, Job),
  ),
  SortableTable.orderedColumn(
    (module Int.Ord),
    ~title="Apartment",
    ~render=v => React.string(Int.Show.show(v)),
    Person.get(_, Apartment),
  ),
  SortableTable.orderedColumn(
    (module JsDate.Ord),
    ~title="Birthday",
    ~render=v => React.string(Js.Date.toDateString(v)),
    Person.get(_, Birthday),
  ),
];

[@react.component]
let make = () => <SortableTable makeRowKey={v => v.Person.id} rows columns />;
