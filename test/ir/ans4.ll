; ModuleID = 'test4.ll'
source_filename = "test4.ll"

define i32 @add() {
entry:
  ret i32 4
}

define i32 @main() {
entry:
  %calltmp = call i32 @add()
  ret i32 %calltmp
}
