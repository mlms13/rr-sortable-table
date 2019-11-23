open BsAbstract.Interface;
open Relude.Globals;

module Column = {
  type key =
    | Key(string);

  type data('row, 'field) = {
    field: 'row => 'field,
    render: 'field => React.element,
    order: option(('field, 'field) => ordering),
  };

  type t('row) =
    | Column(string, key, data('row, 'field)): t('row);

  let makeUnordered =
      (
        type a,
        ~title: string,
        ~key=title,
        ~render: a => React.element,
        field: 'row => a,
      ) =>
    Column(title, Key(key), {field, render, order: None});

  let makeOrdered =
      (
        type a,
        order: (module ORD with type t = a),
        ~title: string,
        ~key=title,
        ~render: a => React.element,
        field: 'row => a,
      ) => {
    module Order = (val order);
    Column(title, Key(key), {field, render, order: Some(Order.compare)});
  };
};

type state('a) =
  | Unsorted
  | Sorted(list('a), [ | `Asc | `Desc], Column.key);

[@react.component]
let make =
    (
      ~makeRowKey: 'row => string,
      ~rows: list('row),
      ~columns: list(Column.t('row)),
    ) => {
  module Table = MaterialUi_Table;
  module TableSortLabel = MaterialUi_TableSortLabel;
  module TableHead = MaterialUi_TableHead;
  module TableBody = MaterialUi_TableBody;
  module TableRow = MaterialUi_TableRow;
  module TableCell = MaterialUi_TableCell;

  let sortRows = (field, sort) =>
    List.sortBy((a, b) => sort(field(a), field(b)));

  let (state, dispatch) =
    React.useReducer(
      state =>
        fun
        // when told to sort with a provided, sortable column, figure out what
        // state we're currently in first
        | `SetSort(Column.Column(_, Key(k), {field, order: Some(order)})) =>
          switch (state) {
          // if the selected column is currently sorted ascending, reverse it
          | Sorted(rows, `Asc, Key(oldKey)) when k == oldKey =>
            Sorted(List.reverse(rows), `Desc, Key(k))

          // if the selected column is currently sorted descending, un-sort
          | Sorted(_, `Desc, Key(oldKey)) when k == oldKey => Unsorted

          // if the selected column isn't currently sorted, sort it (asc)
          | Sorted(_, `Asc, _)
          | Sorted(_, `Desc, Key(_))
          | Unsorted => Sorted(sortRows(field, order, rows), `Asc, Key(k))
          }

        // if we're told to set the sort, but the column doesn't provide
        // ordering, just ignore it (business logic should prevent this)
        | `SetSort(Column.Column(_, _, {order: None})) => state,
      Unsorted,
    );

  let rows =
    switch (state) {
    | Unsorted => rows
    | Sorted(rows, _, _) => rows
    };

  let isSorted = key =>
    switch (state) {
    | Unsorted => false
    | Sorted(_, _, Key(k)) => key == k
    };

  let sortDirection =
    switch (state) {
    | Unsorted => None
    | Sorted(_, dir, _) => Some(dir)
    };

  let headCells =
    List.map(
      fun
      | Column.Column(title, Key(key), {order: Some(_)}) as c =>
        <TableCell key>
          <TableSortLabel
            active={isSorted(key)}
            direction=?sortDirection
            onClick={_ => dispatch(`SetSort(c))}>
            {React.string(title)}
          </TableSortLabel>
        </TableCell>

      | Column.Column(title, Key(key), {order: None}) =>
        <TableCell key> {React.string(title)} </TableCell>,
      columns,
    )
    |> List.toArray
    |> React.array;

  let bodyRows =
    List.map(
      row =>
        <TableRow key={makeRowKey(row)}>
          {List.map(
             (Column.Column(_, Key(key), {field, render})) =>
               <TableCell key={makeRowKey(row) ++ key}>
                 {render(field(row))}
               </TableCell>,
             columns,
           )
           |> List.toArray
           |> React.array}
        </TableRow>,
      rows,
    )
    |> List.toArray
    |> React.array;

  <Table>
    <TableHead> <TableRow> headCells </TableRow> </TableHead>
    <TableBody> bodyRows </TableBody>
  </Table>;
};
