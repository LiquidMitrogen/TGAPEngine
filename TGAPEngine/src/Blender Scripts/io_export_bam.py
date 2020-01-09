
bl_info = {
	"name": "Export BAM format",
	"author": "Szymon Biernacki (mazoot)",
	"version": (1, 1),
	"blender": (2, 71, 0),
	"location": "File > Export > BoneAnimatedModel (.bam)",
	"description": "Export BoneAnimatedModel (.bam)",
	"warning": "",
	"wiki_url": "http://google.com"
				"Scripts/Import-Export/BAM_BoneAnimatedModel_export",
	"category": "Import-Export"}



import bpy
from bpy.props import BoolProperty, IntProperty, EnumProperty
import mathutils
from bpy_extras.io_utils import ExportHelper

from os import remove
import time
import math
import struct
import array
import itertools

def get_weight(group):
	return group.weight

def matrixRtoLTransposed(matrix):
	quatp = matrix.to_quaternion()
	vecp = matrix.to_translation()
	quatk = mathutils.Quaternion((quatp.w,quatp.x,quatp.z,-quatp.y))
	veck = mathutils.Vector((vecp.x,vecp.z,-vecp.y))
	matt = mathutils.Matrix.Translation(veck)
	matr = quatk.to_matrix()
	mat = matt * matr.to_4x4()
	resultMatrix = mat.transposed()
	mathutils.Matrix
	return resultMatrix

def boneDiff(bone,dict,dict2,ar):
	if(bone.parent is None):
		quat1 = mathutils.Quaternion((1.0,0.0,0.0,0.0))
	else:
		quat1 = bone.parent.matrix.to_quaternion()
	dict[bone.name] = bone.matrix.to_quaternion()
	dict2[bone.name] = bone.matrix.to_translation()
	for child in bone.children:
		boneDiff(child,dict,dict2,ar)

def add_to_objectslist(objects,object):
	if(object.type != 'MESH'):
		return False
	objects.append(object)
	ar = object.find_armature()
	if(ar != None):
		if(ar not in objects):
			objects.append(ar)
	for child in object.children:
		add_to_objectslist(objects,child)
	return True

def do_export_bam(context,filepath,props):
	objects = list()
	objectsdict = dict()
	add_to_objectslist(objects,context.active_object)
	for i,selected in enumerate(objects):
		if(selected.type in ('MESH','ARMATURE')):
			objectsdict[selected] = i
			print(selected)
			print(i)
	if(len(objects) == 0):
		return False
	file = open(filepath, "wb")#otworz plik
	print('--------------')
	print(objects)
	print(objectsdict)
	print('----------------')
	file.write(struct.pack('h', 0x4241))#BA
	print('written BA tag')
	file.write(struct.pack('B',len(objects)))
	print('written objects length')
	print(len(objects))
	print('objects')
	for selected,ide in objectsdict.items():
		file.write(struct.pack('B',ide))
		print('written object id')
		print(ide)
		print('has')
		children = list()
		for child in selected.children:
			if(child.type == 'MESH'):
				children.append(child)
		file.write(struct.pack('B',len(children)))
		print('written len of children')
		print(len(children))
		print(' :: ')
		for child in children:
			file.write(struct.pack('B',objectsdict[child]))
			print('written one of children ids')
			print(objectsdict[child])
	for selected,ide in objectsdict.items():
		print('exporting',ide)
		if(selected.type == 'MESH'):
			do_export(file,context,props,selected,ide)
		if(selected.type == 'ARMATURE'):
			do_export_armature(file,context,props,selected,ide)
	file.flush()
	file.close()
	return True
def do_export_armature(file,context,props,object,objectid):
	print("exporting armature")
	ob = object
	ar = object
	sc = context.scene
	####################################################### Header
	file.write(struct.pack('h', 0x4152))#AR
	print('written AR tag')
	file.write(struct.pack('B',objectid))
	file.write(struct.pack('B',0))
	nameCount = len(ob.name)
	ob.rotation_mode = 'QUATERNION'
	file.write(struct.pack('I%ds' % (nameCount) ,nameCount,bytes(ob.name,'ascii')))#object name length and object name
	file.write(struct.pack('fff',ob.location.x,ob.location.z,-ob.location.y))#location of object
	
	obrot = ob.rotation_quaternion
	file.write(struct.pack('ffff',obrot.w,obrot.x,obrot.z,-obrot.y))#rotation of object
	file.write(struct.pack('fff',ob.scale.x,ob.scale.z,ob.scale.y))#scale of object
	file.write(struct.pack('I' ,0))#no vertex_shader or empty
	file.write(struct.pack('I' ,0))#no fragmen_shader or empty
##################################################### /Header
	
	columnFormat = '4f'
	sc.objects.active = ar;
	ar = bpy.context.active_object
	file.write(struct.pack('I',len(ar.pose.bones)))#number of bones
	boneNameToIndex = dict()
	for i,bone in enumerate(ar.pose.bones):
		boneNameToIndex[bone.name] = i
	
	for i,bone in enumerate(ar.pose.bones):
		count = len(bone.name)
		print(count)
		file.write(struct.pack('I%ds' % (count) ,count,bytes(bone.name,'ascii')))#bone name length and bone name
		file.write(struct.pack('I',len(bone.children)))# number of bone children ids
		for child in bone.children:
			file.write(struct.pack('I',boneNameToIndex[child.name]))#bone children ids
		matrix = ar.data.bones[bone.name].matrix_local.inverted()
		print(matrix)
		matrixLT = matrixRtoLTransposed(matrix)
		print(matrixLT)
		for row in matrixLT:
			file.write(struct.pack(columnFormat,*row))#bone armature local parentToBone matrix 
	
	actionString = ar.data.actions
	actions = actionString.split(",")
	actionNum = len(actions)
	file.write(struct.pack('I',actionNum))#number of viable actions for export
	for actionS in actions:
		action = bpy.data.actions[actionS]
		if(action == None):
			print("action not found")
			continue
		actionNameLen = len(action.name)
		print("action ", action.name)
		file.write(struct.pack('I%ds' % (actionNameLen) ,actionNameLen,bytes(action.name,'ascii')))#Action name length and action name
		file.write(struct.pack('I',math.floor(action.frame_range[0])))#action range start
		file.write(struct.pack('I',math.floor(action.frame_range[1])))#action range end
		
		ar.animation_data.action = action
		sc.objects.active = ar;
		ar = bpy.context.active_object
		keyframeCounts=1
		sc.show_keys_from_selected_only = False
		bpy.ops.screen.frame_jump(end = False)
		keyframeIndices = list()
		while True:
			keyframeIndices.append(sc.frame_current)#list of keyframe frame numbers
			result = bpy.ops.screen.keyframe_jump(next = True)
			if(result.pop() != 'FINISHED'):
				break
			keyframeCounts+=1
	
	
		quatFormat = '4f'
		locFormat = '3f'
		
		keyframeToLocDict = dict()
		keyframeToQuatDict = dict()
		for i,v in enumerate(keyframeIndices):
			sc.frame_set(v)
			ar = bpy.context.active_object
			boneNameToQuat = dict()
			boneNameToLoc = dict()
			boneDiff(ar.pose.bones[0],boneNameToQuat,boneNameToLoc,ar) #fills both dictionaries with data from all bones 
			keyframeToQuatDict[v] = boneNameToQuat #each keyframe has complete quats and locs for bone hierarchy in different position
			keyframeToLocDict[v] = boneNameToLoc
		
		for i,bone in enumerate(ar.pose.bones):
			file.write(struct.pack('I',boneNameToIndex[bone.name]))#id of current bone
			file.write(struct.pack('I',keyframeCounts))#number of keyframes for the bone
			bpy.ops.screen.frame_jump(end = False)
			for i,v in enumerate(keyframeIndices):#for every keyframe
				sc.frame_set(v)
				print(v)
				#if(i==0):
				#	rangeMin = v
				#else:
				#	rangeMin = keyframeIndices[i-1] + 1#|s......|.......e|
				#if(i==len(keyframeIndices) - 1):
				#	rangeMax = v
				#else:
				#	rangeMax = keyframeIndices[i+1] - 1
				quat = keyframeToQuatDict[v][bone.name]
				loc =  keyframeToLocDict[v][bone.name]
				print(quat)
				print(loc)
				file.write(struct.pack(quatFormat,quat.w,quat.x,quat.z,-quat.y))#bone pose quaternion in modelspace
				file.write(struct.pack(locFormat,loc.x,loc.z,-loc.y))#bone location in model space
				#file.write(struct.pack('I',rangeMin))
				#file.write(struct.pack('I',rangeMax))
				file.write(struct.pack('I',v))
	
def do_export(file,context, props, object, objectid):
	print("exporting mesh")
	glByte = 0x1400
	glShort = 0x1402
	glInt = 0x1404
	glFloat = 0x1406
	mat_x90 = mathutils.Matrix.Rotation(-math.pi/2, 4, 'X')
	ob = object
	ar = ob.find_armature()
	sc = context.scene
	me = ob.data
	if(ob.get('generate_smart_uvs',-1) == 1):
		bpy.ops.uv.smart_project()
	if(ob.get('export_tangents',-1) == 1):
		me.calc_tangents()
	vertCount = len(me.vertices)
	headerFormat='<12siiffi'
	configurationByte = 0;
	VaoInitStructFormat = 'B16B16I16B16I'
	attrNumber = 4; #vertices, normals, weights and bone indices,texcoords(optional)
	if(ar == None):
		attrNumber = 2;
	if(ob.get('export_textures',-1) == 1):
		
		configurationByte += 1; #0x01 textures
	if(ob.get('export_tangents',-1) == 1):
		
		configurationByte += 2; #0x02 tangents
	if(ar == None):
		configurationByte += 4; #0x04 no bones
####################################################### Header
	file.write(struct.pack('h', 0x4f42))#OB
	print('written OB tag')
	file.write(struct.pack('B',objectid))
	file.write(struct.pack('B',configurationByte))
	nameCount = len(ob.name)
	ob.rotation_mode = 'QUATERNION'
	file.write(struct.pack('I%ds' % (nameCount) ,nameCount,bytes(ob.name,'ascii')))#object name length and object name
	file.write(struct.pack('fff',ob.location.x,ob.location.z,-ob.location.y))#location of object
	
	obrot = ob.rotation_quaternion
	file.write(struct.pack('ffff',obrot.w,obrot.x,obrot.z,-obrot.y))#rotation of object
	file.write(struct.pack('fff',ob.scale.x,ob.scale.z,ob.scale.y))#scale of object
	
	vstr = ob.get('vertex_shader',-1)
	vstrCount = 0
	if (vstr == -1):
		vstrCount = 0
	else:
		vstrCount = len(vstr)
	if vstrCount == 0:
		print('writing 0 to vlen')
		file.write(struct.pack('I' ,0))#no vertex_shader or empty
	else:
		print('writing v ',vstrCount,' len str ',vstr)
		file.write(struct.pack('I%ds' % (vstrCount) ,vstrCount,bytes(vstr,'ascii')))#vertex_shader string
	print('vstr len ', vstrCount)
	print('vstr ', vstr)
	fstr = ob.get('fragment_shader',-1)
	fstrCount = 0
	if (fstr == -1):
		fstrCount = 0
	else:
		fstrCount = len(fstr)
	if fstrCount == 0:
		print('writing 0 to vlen')
		file.write(struct.pack('I' ,0))#no vertex_shader or empty
	else:
		print('writing f ',fstrCount,' len str ',fstr)
		file.write(struct.pack('I%ds' % (fstrCount) ,fstrCount,bytes(fstr,'ascii')))#vertex_shader string
	print('fstr len ', fstrCount)
	print('fstr ', fstr)
##################################################### /Header
	attrSize = [0] * 16
	attrSize[:4] = (3 , 3 , 4 , 4) #3 vertices, 3 normals, 4 weights, 4 bone indices , ? 2 textures, ? 3 tangents, ? 3 bitangents
	attrType = [0] * 16
	attrType[:4] = (glFloat, glFloat, glFloat, glByte)
	attrTypeSize = [0] * 16
	attrTypeSize[:4] = (4,4,4,1)
	attrNormalizeBool = [0] * 16
	attrOffset = [0] * 16
	attrOffset[:4] = (0, vertCount * attrSize[0] * 4, (vertCount * attrSize[0] * 4) + (vertCount * attrSize[1] * 4), (vertCount * attrSize[0] * 4)+(vertCount * attrSize[1] * 4)+(vertCount * attrSize[2] * 4))
	if(ob.get('export_textures',-1) == 1):
		attrNumber += 1
		attrSize[attrNumber-1]=2
		attrType[attrNumber-1]=glFloat 
		attrTypeSize[attrNumber-1] = 4
		attrOffset[attrNumber-1] = 0
		attrOffset[attrNumber-1] += attrOffset[attrNumber - 2] + (vertCount * attrSize[attrNumber - 2] * attrTypeSize[attrNumber - 2])
	if(ob.get('export_tangents',-1) == 1):
		attrNumber += 2
		attrSize[attrNumber-2]=3
		attrType[attrNumber-2]=glFloat
		attrTypeSize[attrNumber-2] = 4
		attrOffset[attrNumber-2] = 0
		attrOffset[attrNumber-2] += attrOffset[attrNumber - 3] + (vertCount * attrSize[attrNumber - 3] * attrTypeSize[attrNumber - 3])
		attrSize[attrNumber-1]=3
		attrType[attrNumber-1]=glFloat
		attrTypeSize[attrNumber-1] = 4
		attrOffset[attrNumber-1] = 0
		attrOffset[attrNumber-1] += attrOffset[attrNumber - 2] + (vertCount * attrSize[attrNumber - 2] * attrTypeSize[attrNumber - 2])
	VaoInitStructStr = struct.pack(VaoInitStructFormat,attrNumber,*(attrSize+attrType+attrNormalizeBool+attrOffset))
	file.write(VaoInitStructStr)
##############################
	buforSize = attrOffset[attrNumber-1] + (vertCount * attrSize[attrNumber-1] * attrTypeSize[attrNumber-1])
	#buforSize = (2 * vertCount * 3 * 4) + (vertCount * 4 * 4) + (vertCount * 4)
	#if(props.export_textures):
		#buforSize = buforSize + (vertCount * 2 * 4)
	file.write(struct.pack('I',buforSize))
	vertexFormat = 'fff'
	for vertex in me.vertices:
		vertexStr = struct.pack(vertexFormat,vertex.co.x,vertex.co.z,-vertex.co.y)
		file.write(vertexStr)
	normalFormat = 'fff'
	for vertex in me.vertices:
		normalStr = struct.pack(normalFormat,vertex.normal.x,vertex.normal.z,-vertex.normal.y)
		file.write(normalStr)
#############################################
	if(ar != None):
		weightFormat = 'ffff'
		for vertex in me.vertices:
			groups = list(vertex.groups)
			weights = list()
			if(len(groups) > 4):
				groups = sorted(groups, key=get_weight, reverse=True)
			for i in range(4):
				try:
					weights.append(groups[i].weight)
				except IndexError:
					weights.append(0)
			#print(weights)
			weightStr = struct.pack(weightFormat,*weights)
			file.write(weightStr)
		weightIndFormat = 'BBBB'
		pair = dict()
		for i,v in enumerate(ar.pose.bones):
			try:
				pair[ob.vertex_groups[v.name].index] = i
			except KeyError:
				print('Key not found (This is normal)')
			#print(pair)
		for vertex in me.vertices:
			groups = list(vertex.groups)
			weightsInd = list()
			if(len(groups) > 4):
				groups = sorted(groups, key=get_weight, reverse=True)
			for i in range(4):
				try:
					weightsInd.append(pair[groups[i].group])
				except IndexError:
					weightsInd.append(0)
			weightIndStr = struct.pack(weightIndFormat,*weightsInd)
			file.write(weightIndStr)
############################# texcoords
	if(ob.get('export_textures',-1) == 1):
		print("exporting texcoords")
		texcoordFormat = 'ff'
		for i,vertex in enumerate(me.vertices):
			for j,o in enumerate(me.uv_layers.active.data):
				if(me.loops[j].vertex_index == i):
					texcoordStr = struct.pack(texcoordFormat,o.uv[0],o.uv[1])
					file.write(texcoordStr)
					break
##############################
	if(ob.get('export_tangents',-1) == 1):
		print("exporting tengents")
		tangentsFormat = 'fff'
		for i,vertex in enumerate(me.vertices):
			xyz = mathutils.Vector((0.0,0.0,0.0))
			for j,loop in enumerate(me.loops):
				if(loop.vertex_index == i):
					xyz += loop.tangent
			xyz.normalize()
			tangentStr = struct.pack(tangentsFormat,xyz.x,xyz.z,-xyz.y)
			file.write(tangentStr)
#############################
	if(ob.get('export_tangents',-1) == 1):
		bitangentFormat = 'fff'
		for i,vertex in enumerate(me.vertices):
			xyz = mathutils.Vector((0.0,0.0,0.0))
			for j,loop in enumerate(me.loops):
				if(loop.vertex_index == i):
					xyz += loop.bitangent
			xyz.normalize()
			bitangentStr = struct.pack(bitangentFormat,xyz.x,xyz.z,-xyz.y)
			file.write(bitangentStr)
#############################
	indicesCountFormat = 'I'
	file.write(struct.pack(indicesCountFormat,(len(me.polygons) * 3 * 4)))
	indicesFormat = 'III'
	for triangle in me.polygons:
		indicesStr = struct.pack(indicesFormat,*triangle.vertices) 
		file.write(indicesStr)
###########################
	if(ar == None):
		return True
##########################################
	arNameCount = len(ar.name)
	file.write(struct.pack('I%ds' % (arNameCount) ,arNameCount,bytes(ar.name,'ascii')))#armature name length and object name
	return True


###### EXPORT OPERATOR #######
class Export_bam(bpy.types.Operator, ExportHelper):
	"""Export the active Object as a .bam file"""
	bl_idname = "export_shape.bam"
	bl_label = "Export bam (.bam)"

	filename_ext = ".bam"

	export_textures = BoolProperty(name="Export texture coordinates",
			description="Adds texture coordinates to export process",
			default=False,
			)
	export_tangents = BoolProperty(name="Export tangent and bitangent coords",
			description="Adds tangent and bitangent coordinates to export process",
			default=False,
			)
	generate_smart_uvs = BoolProperty(name="Generate smart UVs on export",
			description="Adds UV generation to export process",
			default=False,
			)

	@classmethod
	def poll(cls, context):
		return context.active_object.type in {'MESH', 'CURVE', 'SURFACE', 'FONT'}

	def execute(self, context):
		start_time = time.time()
		print('\n_____START_____')
		props = self.properties
		filepath = self.filepath
		filepath = bpy.path.ensure_ext(filepath, self.filename_ext)

		exported = do_export_bam(context, filepath, props)

		if exported:
			print('finished export in %s seconds' %((time.time() - start_time)))
			print(filepath)

		return {'FINISHED'}

	def invoke(self, context, event):
		wm = context.window_manager

		if True:
			# File selector
			wm.fileselect_add(self) # will run self.execute()
			return {'RUNNING_MODAL'}
		elif True:
			# search the enum
			wm.invoke_search_popup(self)
			return {'RUNNING_MODAL'}
		elif False:
			# Redo popup
			return wm.invoke_props_popup(self, event)
		elif False:
			return self.execute(context)


### REGISTER ###



def menu_func(self, context):
	self.layout.operator(Export_bam.bl_idname, text="BoneAnimatedModel (.bam)")


def register():
	bpy.utils.register_module(__name__)

	bpy.types.INFO_MT_file_export.append(menu_func)
	#bpy.types.VIEW3D_PT_tools_objectmode.prepend(menu_func)

def unregister():
	bpy.utils.unregister_module(__name__)

	bpy.types.INFO_MT_file_export.remove(menu_func)
	#bpy.types.VIEW3D_PT_tools_objectmode.remove(menu_func)

if __name__ == "__main__":
	register()
