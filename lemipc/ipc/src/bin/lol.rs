// #[repr(C)]
// pub struct Struct<T: ?Sized> {
//     pub size: i64,
//     pub buf: T,
// }

// // how to create raw pointer from ref???
// fn send<T: ?Sized>(dst: Box<Msg<[u8]>>) {
//     let ptr = Box::into_raw(dst) as *const c_void;

//     unsafe {
//         // void c_foo(void *ptr);
//         c_foo(ptr);
//     }
// }

// fn main() {
//     let dst = Box::new(Struct {
//         size: 5,
//         buf: b"test".clone()
//     });

//     send(dst);
//     // send(&dst);
//     // i want to use dst after all of this
// }

fn main() {

}