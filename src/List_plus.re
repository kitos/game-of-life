let count = (pred: 'a => bool, l: list('a)) =>
  List.fold_left((total, v) => pred(v) ? total + 1 : total + 0, 0, l);
