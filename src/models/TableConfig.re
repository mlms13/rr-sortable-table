module type T = {
  type field('a);
  type data;
  let set: (data, field('a), 'a) => data;
  let get: (data, field('a)) => 'a;
};
