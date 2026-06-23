; ModuleID = 'actual_out/actual_out5.ll'
source_filename = "actual_out/actual_out5.ll"

define i32 @main() {
entry:
  %x = alloca i32
  store i32 2, ptr %x


  ret i32 0
}
