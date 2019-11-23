module Person = [%lenses
  type data = {
    id: string,
    name: string,
    job: option(string),
    apartment: int,
    birthday: Js.Date.t,
  }
];

let sample =
  Person.[
    {
      id: "0",
      name: "Dave Ray",
      job: Some("Diner"),
      apartment: 68,
      birthday: Js.Date.makeWithYMD(~year=1980., ~month=2., ~date=6., ()),
    },
    {
      id: "1",
      name: "F. Morris",
      job: Some("Floral Studio"),
      apartment: 58,
      birthday: Js.Date.makeWithYMD(~year=1974., ~month=0., ~date=13., ()),
    },
    {
      id: "2",
      name: "Melissa Ford",
      job: None,
      apartment: 56,
      birthday: Js.Date.makeWithYMD(~year=1987., ~month=10., ~date=24., ()),
    },
    {
      id: "3",
      name: "C. Henderson",
      job: None,
      apartment: 62,
      birthday: Js.Date.makeWithYMD(~year=1996., ~month=11., ~date=24., ()),
    },
    {
      id: "4",
      name: "Sara Fox",
      job: Some("Hat Shop"),
      apartment: 58,
      birthday: Js.Date.makeWithYMD(~year=1991., ~month=6., ~date=2., ()),
    },
    {
      id: "5",
      name: "Oscar Griffin",
      job: Some("Bike Shop"),
      apartment: 61,
      birthday: Js.Date.makeWithYMD(~year=1981., ~month=1., ~date=24., ()),
    },
    {
      id: "6",
      name: "Mae Chapman",
      job: Some("Graphic Design"),
      apartment: 62,
      birthday: Js.Date.makeWithYMD(~year=1989., ~month=7., ~date=24., ()),
    },
    {
      id: "7",
      name: "Timothy Austin",
      job: None,
      apartment: 63,
      birthday: Js.Date.makeWithYMD(~year=1984., ~month=0., ~date=3., ()),
    },
    {
      id: "8",
      name: "Jennifer Hunt",
      job: Some("Diner"),
      apartment: 56,
      birthday: Js.Date.makeWithYMD(~year=1995., ~month=9., ~date=18., ()),
    },
  ];
