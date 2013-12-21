require 'sketchup'
require 'SkpCol'

$coll == nil

if not $skp_coll_loaded
  $skp_coll_loaded = true
  Sketchup.send_action "showRubyPanel:"

  UI.menu('plugins').add_item('testcoll!') { test_skp_coll }
end

def test_skp_coll
  if $coll == nil
    $coll = CColl::Coll.new('test')
  end
  puts $coll.load_model(Sketchup.active_model)
end
