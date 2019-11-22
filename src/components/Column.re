open BsAbstract.Interface;

module Make = (Config: TableConfig.T) => {
  type key =
    | Key(string);

  type data('a) = {
    field: Config.field('a),
    render: 'a => React.element,
    order: option(('a, 'a) => ordering),
  };

  type t =
    | Column(string, key, data('a)): t;

  let makeUnordered =
      (
        type a,
        ~title: string,
        ~key=title,
        ~render: a => React.element,
        field: Config.field(a),
      ) =>
    Column(title, Key(key), {field, render, order: None});

  let makeOrdered =
      (
        type a,
        order: (module ORD with type t = a),
        ~title: string,
        ~key=title,
        ~render: a => React.element,
        field: Config.field(a),
      ) => {
    module Order = (val order);
    Column(title, Key(key), {field, render, order: Some(Order.compare)});
  };
};
