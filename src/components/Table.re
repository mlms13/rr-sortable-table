open Relude.Globals;

module Make = (Config: TableConfig.T) => {
  module Column = Column.Make(Config);

  [@react.component]
  let make =
      (
        ~makeKey: Config.data => string,
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
        | Column.Column(title, {field, order: Some(order)}) =>
          <TableCell key=title>
            <TableSortLabel onClick={_ => sortBy(field, order)}>
              {React.string(title)}
            </TableSortLabel>
          </TableCell>

        | Column.Column(title, {order: None}) =>
          <TableCell key=title> {React.string(title)} </TableCell>,
        columns,
      )
      |> List.toArray
      |> React.array;

    let bodyRows =
      List.map(
        row =>
          <TableRow key={makeKey(row)}>
            {List.map(
               (Column.Column(_, {field, show})) =>
                 <TableCell>
                   {React.string(show(Config.get(row, field)))}
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
