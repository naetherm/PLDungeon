Most important first
	Do NOT EVER just take work from another one and put in into this project. This is a public project, so we REALLY need to respect copyright
	to avoid legal issues. Do always add information about the source or author, even if you are the autor. Do only use free usable data, your
	own data included!


Main directories
- "Bin": 		The "final product"
- "Source":		Source codes
- "Workspace":	Original uncompressed data and so on


"Workspace" directory
- "AuthorsAndSources":		Information were data came from
- "DataCrypt":				Recycle bin
- "Documentation":			If there is something helpful to be written down, put in in here
- "OriginalSounds":			Original sounds
- "Renderings":				Reference renderings
- "OriginalTextures":		Original sources
- "UncompressedTextures":	Textures in a uncompressed form, do only work on those files and not on compressed (with quality loss) data!


Texture name convention
	<author/source>_<descriptive name>_<map type>
	-> <author/source> - More information about the author/source can be found within the "AuthorsAndSources" directory
	-> It's only allowed to break this convention if the source, the data came from already had a kind of name convention and it's easier to
       find the orignal data when using the orignal name


3ds Max
	- File: "Bin\"
	- Export: Used name "Dungeon", dark ambient color (everything else: default settings)


Lookout caches!
- To speed up loading
	<Container Class="PLPhysics::SCPhysicsWorld" Name="Container" Flags="CastShadow">
  was changed within the exported scene to
	<Container Class="PLPhysics::SCPhysicsWorld" Name="Container" Flags="CastShadow" CacheDirectory="_Cache/PLPhysicsNewton">
  This allows the physics backend PLPhysicsNewton to create the physics meshes just once, and then just load them the next time.
  Depending on the OS and mesh complexity, this influences the loading time dramatically...
  ... but when changing the meshes, DON'T forget do delete the cache, else the graphical meshes may differ from the physics meshes!
