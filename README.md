# Reason React Sortable Table

This is an exploration in making a generic Reason React table component. Each row is of some type `'a` (which is probably some product type like a record, but really could be anything). Columns are also defined in a list.

Each column:

- has a title
- uses the title as its React key, but allows overriding the key to avoid collisions
- knows how to extract a value from the row data type
- knows how to render values of that type
- optionally knows how to order values of that type, which will make the column sortable

Normally, this would mean that a column is polymorphic (it extracts and renders different types of values from the row data), but that would make it difficult to provide a `list(column('a))` because each column would need to operate on the same type of cell data. Instead, `column` is defined as a GADT with an existential type, so your sort and render functions are type-safe, even though that type isn't represented in the column type.

## Usage

Of note, this is intended to be a source of inspiration more than a ready-to-use library. At the moment, I'm not planning on packaging this up and releasing it on npm, but if you're already using Material UI and want a simple sortable table component, usage looks like this:

```reason
open Relude.Globals;
open BsAbstract;

type role = Admin | User;

let roleToString =
  fun
  | Admin => "Admin"
  | User => "User";

type user = {
  id: string,
  name: string,
  roles: list(role),
  birthday: option(Js.Date.t),
};

let rows = [ /* include user records here... */ ];

let columns = [
  SortableTable.orderedColumn(
    (module String.Ord),
    ~title="Name",
    ~render=React.string,
    user => user.name,
  ),
  SortableTable.unorderedColumn(
    ~title="Roles",
    ~render=
      roles =>
        roles
        |> List.map(roleToString)
        |> List.String.joinWith(", ")
        |> React.string,
    user => user.roles,
  ),
  SortableTable.orderedColumn(
    (module Option.Ord(JsDate.Ord)),
    ~title="Birthday",
    ~render=bd => Option.fold("-", Js.Date.toString, bd) |> React.string,
    user => user.birthday,
  )
];

[@react.component]
let make = () => <SortableTable makeRowKey={user => user.id} rows columns />;
```

The above example assumes you're using the [Relude](https://github.com/reazen/relude) + [bs-abstract](https://github.com/reazen/relude) libraries. The implementation of `SortableTable` in this project asks orderable columns be constructed with an `Ord` module for the corresponding data type. bs-abstract provides these `Ord` modules for many common types.

But if Relude and bs-abstract aren't your cup of tea, that's fine -- the implementation could absolutely use a "normal" `('a, 'a) => int` function for ordering instead, as is common in the OCaml world. Feel free to hack up this idea until it works for your needs.

Earlier versions relied on [lenses-ppx](https://github.com/Astrocoders/lenses-ppx/) to generate the getters for each column. That has since been simplified away, but the ppx is still very useful, as you can see [in the example](https://github.com/mlms13/rr-sortable-table/tree/master/examples/users).

## Limitations

- The table maintains its own state (to track the current sort). It caches the currently-sorted rows, which means that if you pass in updated row data, it won't be immediately picked up. We could work around this pretty easily with a `useEffect` hook that watches the rows.

- Material UI may not be for everyone. This approach could be modified to work with basically any table rendering. I happened to pick Material UI because it's easy enough and I didn't want to write CSS.
