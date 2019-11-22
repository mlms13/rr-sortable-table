open BsAbstract.Interface;

// This will go away entirely once my PR to bs-abstract is merged
module Ord: (O: ORD) => ORD with type t = option(O.t) =
  (O: ORD) => {
    include BsAbstract.Option.Eq(O);

    let compare = (a, b) =>
      switch (a, b) {
      | (Some(a), Some(b)) => O.compare(a, b)
      | (None, None) => `equal_to
      | (None, Some(_)) => `less_than
      | (Some(_), None) => `greater_than
      };
  };
