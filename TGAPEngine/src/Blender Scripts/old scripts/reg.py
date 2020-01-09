bl_info = {
	"name": "Add BAMS Properties",
	"category": "Object",
}
import bpy
class ObjectBamsProperties(bpy.types.Operator):
	"""Object Bams Properties"""
	bl_idname = "object.add_bams_properties"
	bl_label = "Add Bams Properties"
	bl_options = {'REGISTER'}
	generate_smart_uvs = bpy.props.IntProperty(name="generate_smart_uvs", default = 0, min = 0, max = 1)
	export_tangents = bpy.props.IntProperty(name="export_tangents", default = 0, min = 0, max = 1)
	export_textures = bpy.props.IntProperty(name="export_textures", default = 0, min = 0, max = 1)
	vertex_shader = bpy.props.StringProperty(name = "vertex_shader_path", subtype='FILE_PATH')
	fragment_shader = bpy.props.StringProperty(name = "fragment_shader_path", subtype='FILE_PATH')
	def execute(self, context):
		obj = context.active_object
		obj.generate_smart_uvs = self.generate_smart_uvs
		obj.export_tangents = self.export_tangents
		obj.export_textures = self.export_textures
		obj.vertex_shader = self.vertex_shader
		obj.fragment_shader = self.fragment_shader
		return {'FINISHED'}
def menu_func(self, context):
	self.layout.operator(ObjectBamsProperties.bl_idname)
# store keymaps here to access after registration
addon_keymaps = []
def register():
	bpy.types.Object.generate_smart_uvs = bpy.props.IntProperty(name="generate_smart_uvs", default = 0, min = 0, max = 1)
	bpy.types.Object.export_tangents = bpy.props.IntProperty(name="export_tangents", default = 0, min = 0, max = 1)
	bpy.types.Object.export_textures = bpy.props.IntProperty(name="export_textures", default = 0, min = 0, max = 1)
	bpy.types.Object.vertex_shader = bpy.props.StringProperty(name = "vertex_shader_path", subtype='FILE_PATH')
	bpy.types.Object.fragment_shader = bpy.props.StringProperty(name = "fragment_shader_path", subtype='FILE_PATH')
	bpy.utils.register_class(ObjectBamsProperties)
	bpy.types.VIEW3D_MT_object.append(menu_func)
	# handle the keymap
	wm = bpy.context.window_manager
	# Note that in background mode (no GUI available), keyconfigs are not available either, so we have to check this
	# to avoid nasty errors in background case.
	kc = wm.keyconfigs.addon
	if kc:
		km = wm.keyconfigs.addon.keymaps.new(name='Object Mode', space_type='EMPTY')
		kmi = km.keymap_items.new(ObjectBamsProperties.bl_idname, 'SPACE', 'PRESS', ctrl=True, shift=True)
		addon_keymaps.append((km, kmi))
def unregister():
	# Note: when unregistering, it's usually good practice to do it in reverse order you registered.
	# Can avoid strange issues like keymap still referring to operators already unregistered...
	# handle the keymap
	for km, kmi in addon_keymaps:
		km.keymap_items.remove(kmi)
	addon_keymaps.clear()
	bpy.utils.unregister_class(ObjectBamsProperties)
	bpy.types.VIEW3D_MT_object.remove(menu_func)
if __name__ == "__main__":
	register()