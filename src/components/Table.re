open Relude.Globals;

module Make = (Config: TableConfig.T) => {
  module Column = Column.Make(Config);

  type dir =
    | Asc
    | Desc;

  type state =
    | Unsorted
    | Sorted(list(Config.data), dir, Column.key);

  [@react.component]
  let make =
      (
        ~makeRowKey: Config.data => string,
        ~data: list(Config.data),
        ~columns: list(Column.t),
      ) => {
    module Table = MaterialUi_Table;
    module TableSortLabel = MaterialUi_TableSortLabel;
    module TableHead = MaterialUi_TableHead;
    module TableBody = MaterialUi_TableBody;
    module TableRow = MaterialUi_TableRow;
    module TableCell = MaterialUi_TableCell;

    let sortRows = (field, sort) =>
      List.sortBy((a, b) =>
        sort(Config.get(a, field), Config.get(b, field))
      );

    let (state, dispatch) =
      React.useReducer(
        state =>
          fun
          // when told to sort with a provided, sortable column, figure out what
          // state we're currently in first
          | `SetSort(Column.Column(_, Key(k), {field, order: Some(order)})) =>
            switch (state) {
            // if the selected column is currently sorted ascending, reverse it
            | Sorted(rows, Asc, Key(oldKey)) when k == oldKey =>
              Sorted(List.reverse(rows), Desc, Key(k))

            // if the selected column is currently sorted descending, un-sort
            | Sorted(_, Desc, Key(oldKey)) when k == oldKey => Unsorted

            // if the selected column isn't currently sorted, sort it (asc)
            | Sorted(_, Asc, _)
            | Sorted(_, Desc, Key(_))
            | Unsorted => Sorted(sortRows(field, order, data), Asc, Key(k))
            }

          // if we're told to set the sort, but the column doesn't provide
          // ordering, just ignore it (business logic should prevent this)
          | `SetSort(Column.Column(_, _, {order: None})) => state,
        Unsorted,
      );

    let rows =
      switch (state) {
      | Unsorted => data
      | Sorted(rows, _, _) => rows
      };

    let headCells =
      List.map(
        fun
        | Column.Column(title, Key(key), {order: Some(_)}) as c =>
          <TableCell key>
            <TableSortLabel onClick={_ => dispatch(`SetSort(c))}>
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
                   {render(Config.get(row, field))}
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
};
