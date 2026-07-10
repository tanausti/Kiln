; ModuleID = 'actual_out/actual_out6.ll'
source_filename = "actual_out/actual_out6.ll"

define i32 @main() {
entry:
  br i1 true, label %then, label %end

then:                                             ; preds = %entry
  ret i32 2

end:                                              ; preds = %entry
  ret i32 3
}
