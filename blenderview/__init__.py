import bpy
import os

bl_info = {
	"name": "3D particles - blenderview",
	"author": "Scienceproject team",
	"version": (1, 0, 0),
	"blender": (2, 80, 0),
	"location": "3D Viewport > Sidebar > 3D particles",
	"description": "3D particles results viewer for Blender",
	"category": "Render"
}

class particles:
	class properties(bpy.types.PropertyGroup):
		comment : bpy.props.StringProperty(name="")
		folder_path : bpy.props.StringProperty(name="Path", subtype="FILE_PATH")
		color : bpy.props.FloatVectorProperty(name="Color", subtype="COLOR", min=0.0, max=1.0, default=(0.0, 1.0, 0.0))
		import_particle_num : bpy.props.IntProperty(name="", subtype="UNSIGNED")
		import_group_num : bpy.props.IntProperty(name="", subtype="UNSIGNED")
		mode = 0 # 0: none | 1: all | 2: particle | 3: group
	
	class panel(bpy.types.Panel):
		bl_space_type = "VIEW_3D"
		bl_region_type = "UI"
		bl_category = "3D particles"
		bl_label = "3D particles"
		
		def draw(self, context):
			properties = context.scene.ParticlesProperties
			
			row = self.layout.row()
			row.label(text=properties.comment)
			row = self.layout.row()
			row.operator("particles.init", text="(re-)init")
			row = self.layout.row()
			row.prop(properties, "folder_path")
			row = self.layout.row()
			row.prop(properties, "color")
			self.layout.separator()
			row = self.layout.row()
			row.operator("import_obj.all", text="import all")
			row.enabled = True if properties.mode == 0 else False
			row = self.layout.row()
			row.operator("import_obj.particle", text="import particle")
			row.prop(properties, "import_particle_num")
			row.enabled = True if properties.mode in [0, 2] else False
			row = self.layout.row()
			row.operator("import_obj.group", text="import group")
			row.prop(properties, "import_group_num")
			row.enabled = True if properties.mode in [0, 3] else False
	
	class init(bpy.types.Operator):
		"""(re-)init the 3D particles view"""
		bl_idname = "particles.init"
		bl_label = "(re-)init the view"
		
		def execute(self, context):
			for ob in bpy.data.objects:
				ob.hide_set(False)
				ob.hide_viewport = False
				ob.hide_select = False
			bpy.ops.object.select_all(action="SELECT")
			bpy.ops.object.delete(use_global=False)
			bpy.data.scenes["Scene"].frame_end = 0
			context.scene.ParticlesProperties.comment = "Scene ready!"
			context.scene.ParticlesProperties.mode = 0
			return {"FINISHED"}
	
	class import_obj:
		class all(bpy.types.Operator):
			"""Import all 3D particles"""
			bl_idname = "import_obj.all"
			bl_label = "import all"
			def execute(self, context):
				bpy.ops.particles.init()
				context.scene.ParticlesProperties.mode = 1
				folder_path = context.scene.ParticlesProperties.folder_path
				if not particles.import_obj.exec(folder_path, "all"): return {"CANCELLED"}
				# animation
				bpy.data.scenes["Scene"].frame_end = 0
				min1, min2 = 10000, 10000
				for ob in bpy.data.objects:
					i = float(ob.name[3:])
					if min1 > i: min2 = min1; min1 = i
					elif min2 > i: min2 = i
				delta = min2 - min1
				# TODO : material = particles.import_obj.geometry_node()
				for ob in bpy.data.objects:
					i = int(float(ob.name[3:]) / delta)
					ob.hide_viewport = True
					ob.keyframe_insert(data_path="hide_viewport", frame=0)
					ob.keyframe_insert(data_path="hide_viewport", frame=i*3-1)
					ob.keyframe_insert(data_path="hide_viewport", frame=i*3+3)
					ob.hide_viewport = False
					ob.keyframe_insert(data_path="hide_viewport", frame=-1)
					ob.keyframe_insert(data_path="hide_viewport", frame=i*3)
					ob.keyframe_insert(data_path="hide_viewport", frame=i*3+2)
					if bpy.data.scenes["Scene"].frame_end < i*3+2: bpy.data.scenes["Scene"].frame_end = int(i*3+2)
				context.scene.ParticlesProperties.comment = "Finished!"
				return {"FINISHED"}
		
		class particle(bpy.types.Operator):
			"""Import a 3D particle"""
			bl_idname = "import_obj.particle"
			bl_label = "import particle"
			def execute(self, context):
				bpy.ops.particles.init()
				context.scene.ParticlesProperties.mode = 2
				bpy.ops.wm.obj_import(filepath=context.scene.ParticlesProperties.folder_path+"p"+str(context.scene.ParticlesProperties.import_particle_num)+".obj", up_axis="Z", forward_axis="Y")
				context.scene.ParticlesProperties.comment = "Finished!"
				return {"FINISHED"}
		
		class group(bpy.types.Operator):
			"""Import 3D particles group"""
			bl_idname = "import_obj.group"
			bl_label = "import group"
			def execute(self, context):
				context.scene.ParticlesProperties.mode = 3
				folder_path = context.scene.ParticlesProperties.folder_path
				group_num = context.scene.ParticlesProperties.import_group_num
				prefix = "group"+str(group_num)+"_"
				if not particles.import_obj.exec(folder_path, prefix): return {"CANCELLED"}
				# animation
				min1, min2 = 10000, 10000
				for ob in bpy.data.objects:
					if ob.name.startswith(prefix):
						i = float(ob.name[len(prefix):])
						if min1 > i: min2 = min1; min1 = i
						elif min2 > i: min2 = i
				delta = min2 - min1
				for ob in bpy.data.objects:
					if ob.name.startswith(prefix):
						i = int(float(ob.name[ob.name.find("_")+1:]) / delta)
						ob.hide_viewport = True
						ob.keyframe_insert(data_path="hide_viewport", frame=0)
						ob.keyframe_insert(data_path="hide_viewport", frame=i*3-1)
						ob.keyframe_insert(data_path="hide_viewport", frame=i*3+3)
						ob.hide_viewport = False
						ob.keyframe_insert(data_path="hide_viewport", frame=-1)
						ob.keyframe_insert(data_path="hide_viewport", frame=i*3)
						ob.keyframe_insert(data_path="hide_viewport", frame=i*3+2)
						if bpy.data.scenes["Scene"].frame_end < i*3+2: bpy.data.scenes["Scene"].frame_end = int(i*3+2)
				context.scene.ParticlesProperties.comment = "Finished!"
				return {"FINISHED"}
		
		def exec(folder_path, prefix):
			if not os.path.exists(folder_path):
				context.scene.ParticlesProperties.comment = "Path doesn't exsist!"
				return False
			obj_files = [(folder_path+f) for f in os.listdir(folder_path) if f.endswith(".obj") and f.startswith(prefix)]
			for obj_file in obj_files:
				bpy.ops.wm.obj_import(filepath=obj_file, up_axis="Z", forward_axis="Y")
			return True
		
		"""def geometry_node(objects):
			bpy.ops.material.new()
			material = bpy.data.materials[-1].node_tree
			bsdf = material.nodes.new(type="ShaderNodeBsdfPrincipled")
			bsdf.inputs[0].default_value = (context.scene.ParticlesProperties.color.r, context.scene.ParticlesProperties.color.g, context.scene.ParticlesProperties.color.b, 1)
			material.links.new(bsdf.outputs["BSDF"], material.nodes.new(type='ShaderNodeOutputMaterial').inputs['Surface'])
			for obj in objects:
				mod = obj.modifiers.new("3DparticlesGeometryNodes", "NODES")
				mod.add_node(use_transform=True, type="GeometryNodeInstanceOnPoints")
				ico = mod.add_node(use_transform=True, type="GeometryNodeMeshIcoSphere")
				mod.links.new(ico.outputs[)
			return material"""

def register():
	bpy.utils.register_class(particles.properties)
	bpy.utils.register_class(particles.panel)
	bpy.utils.register_class(particles.init)
	bpy.utils.register_class(particles.import_obj.all)
	bpy.utils.register_class(particles.import_obj.particle)
	bpy.utils.register_class(particles.import_obj.group)
	
	bpy.types.Scene.ParticlesProperties = bpy.props.PointerProperty(type=particles.properties)

def unregister():
	bpy.utils.unregister_class(particles.properties)
	bpy.utils.unregister_class(particles.panel)
	bpy.utils.unregister_class(particles.init)
	bpy.utils.unregister_class(particles.import_obj.all)
	bpy.utils.unregister_class(particles.import_obj.particle)
	bpy.utils.unregister_class(particles.import_obj.group)
	
	del bpy.types.Scene.ParticlesProperties

if __name__ == "__main__":
	register()