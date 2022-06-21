target_sources(whirl PRIVATE
  whirl/builtin.c
  whirl/reader.c
  whirl/tir.c
  whirl/value.c
  whirl/list.c)

target_sources(whirl.cli PRIVATE
  whirl/wrl-main.c)
