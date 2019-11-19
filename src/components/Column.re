open BsAbstract.Interface;

module Make = (Config: TableConfig.T) => {
  type data('a) = {
    field: Config.field('a),
    show: 'a => string,
    order: option(('a, 'a) => ordering),
  };

  type t =
    | Column(string, data('a)): t;

  let makeUnordered =
      (
        type a,
        ~title: string,
        ~showLabel: a => string,
        field: Config.field(a),
      ) =>
    Column(title, {field, show: showLabel, order: None});

  let makeOrdered =
      (
        type a,
        order: (module ORD with type t = a),
        ~title: string,
        ~showLabel: a => string,
        field: Config.field(a),
      ) => {
    module Order = (val order);
    Column(title, {field, show: showLabel, order: Some(Order.compare)});
  };
};
