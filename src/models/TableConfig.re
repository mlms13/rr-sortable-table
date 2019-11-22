module type T = {
  type field('a);
  type data;
  let get: (data, field('a)) => 'a;
};
