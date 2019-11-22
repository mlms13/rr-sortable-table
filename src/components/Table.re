open Relude.Globals;

module Make = (Config: TableConfig.T) => {
  module Column = Column.Make(Config);

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

    let (sorted, setSorted) = React.useState(() => data);

    let sortBy = (field, sort) =>
      setSorted(_state =>
        List.sortBy(
          (a, b) => sort(Config.get(a, field), Config.get(b, field)),
          sorted,
        )
      );

    let headCells =
      List.map(
        fun
        | Column.Column(title, Key(key), {field, order: Some(order)}) =>
          <TableCell key>
            <TableSortLabel onClick={_ => sortBy(field, order)}>
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
        sorted,
      )
      |> List.toArray
      |> React.array;

    <Table>
      <TableHead> <TableRow> headCells </TableRow> </TableHead>
      <TableBody> bodyRows </TableBody>
    </Table>;
  };
};
