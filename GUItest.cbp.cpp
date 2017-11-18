<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<CodeBlocks_project_file>
	<FileVersion major="1" minor="6" />
	<Project>
		<Option title="TINS2017" />
		<Option pch_mode="2" />
		<Option compiler="gcc" />
		<Build>
			<Target title="Debug">
				<Option output="cbbuild/bin/Libtest-debug" prefix_auto="1" extension_auto="1" />
				<Option working_dir="cbbuild/bin" />
				<Option object_output="cbbuild/obj/Dynamic/Debug" />
				<Option external_deps="cbbuild/lib/libeagle_a5_debug.dll.a;cbbuild/lib/libeagle_debug.dll.a;" />
				<Option type="1" />
				<Option compiler="gcc" />
				<Compiler>
					<Add option="-O0" />
					<Add option="-ggdb3" />
					<Add option="-DDEBUG" />
					<Add option="-DEAGLE_LOG" />
					<Add option="-DEAGLE_ASSERT_EXCEPTION" />
				</Compiler>
				<Linker>
					<Add option="-leagle_a5_debug.dll" />
					<Add option="-leagle_debug.dll" />
					<Add option="-lallegro_monolith-debug.dll" />
				</Linker>
			</Target>
			<Target title="Release">
				<Option output="cbbuild/bin/Libtest" prefix_auto="1" extension_auto="1" />
				<Option working_dir="cbbuild/bin" />
				<Option object_output="cbbuild/obj/Dynamic/Release" />
				<Option external_deps="cbbuild/lib/libeagle_a5.dll.a;cbbuild/lib/libeagle.dll.a;" />
				<Option type="1" />
				<Option compiler="gcc" />
				<Compiler>
					<Add option="-O2" />
					<Add option="-DDEBUG" />
				</Compiler>
				<Linker>
					<Add option="-s" />
					<Add option="-leagle_a5.dll" />
					<Add option="-leagle.dll" />
					<Add option="-lallegro_monolith.dll" />
				</Linker>
			</Target>
			<Target title="Profiling">
				<Option output="cbbuild/bin/Libtest-profile" prefix_auto="1" extension_auto="1" />
				<Option working_dir="cbbuild/bin" />
				<Option object_output="cbbuild/obj/Dynamic/Profiling" />
				<Option external_deps="cbbuild/lib/libeagle_a5_profile.dll.a;cbbuild/lib/libeagle_profile.dll.a;" />
				<Option type="1" />
				<Option compiler="gcc" />
				<Compiler>
					<Add option="-O2" />
					<Add option="-pg" />
				</Compiler>
				<Linker>
					<Add option="-pg -lgmon" />
					<Add option="-leagle_a5_profile.dll" />
					<Add option="-leagle_profile.dll" />
					<Add option="-lallegro_monolith.dll" />
				</Linker>
			</Target>
			<Target title="StaticDebug">
				<Option output="cbbuild/bin/Libtest-debug-static" prefix_auto="1" extension_auto="1" />
				<Option working_dir="cbbuild/bin" />
				<Option object_output="cbbuild/obj/Static/Debug" />
				<Option external_deps="cbbuild/lib/libeagle_a5_debug-static.a;cbbuild/lib/libeagle_debug-static.a;" />
				<Option type="1" />
				<Option compiler="gcc" />
				<Compiler>
					<Add option="-O0" />
					<Add option="-ggdb3" />
					<Add option="-DALLEGRO_STATICLINK" />
					<Add option="-DDEBUG" />
					<Add option="-DEAGLE_LOG" />
					<Add option="-DEAGLE_ASSERT_EXCEPTION" />
				</Compiler>
				<Linker>
					<Add option="-static-libstdc++" />
					<Add option="-static-libgcc" />
					<Add option="-static" />
					<Add option="-leagle_a5_debug-static" />
					<Add option="-leagle_debug-static" />
					<Add option="-lallegro_monolith-debug-static" />
					<Add option="-lFLAC.dll" />
					<Add option="-ljpeg" />
					<Add option="-ldumb" />
					<Add option="-lvorbisfile" />
					<Add option="-lvorbis" />
					<Add option="-lfreetype" />
					<Add option="-logg" />
					<Add option="-lpng16" />
					<Add option="-lzlibstatic" />
					<Add option="-lgdiplus" />
					<Add option="-luuid" />
					<Add option="-lkernel32" />
					<Add option="-lwinmm" />
					<Add option="-lpsapi" />
					<Add option="-ldsound" />
					<Add option="-ld3dx9" />
					<Add option="-lopengl32" />
					<Add option="-lglu32" />
					<Add option="-luser32" />
					<Add option="-lcomdlg32" />
					<Add option="-lgdi32" />
					<Add option="-lshell32" />
					<Add option="-lole32" />
					<Add option="-ladvapi32" />
					<Add option="-lws2_32" />
					<Add option="-lshlwapi" />
					<Add option="-lgcc_eh" />
				</Linker>
			</Target>
			<Target title="StaticRelease">
				<Option output="cbbuild/bin/Libtest-static" prefix_auto="1" extension_auto="1" />
				<Option working_dir="cbbuild/bin" />
				<Option object_output="cbbuild/obj/Static/Release" />
				<Option external_deps="cbbuild/lib/libeagle_a5-static.a;cbbuild/lib/libeagle-static.a;" />
				<Option type="1" />
				<Option compiler="gcc" />
				<Compiler>
					<Add option="-O2" />
					<Add option="-DALLEGRO_STATICLINK" />
				</Compiler>
				<Linker>
					<Add option="-s" />
					<Add option="-static-libstdc++" />
					<Add option="-static-libgcc" />
					<Add option="-static" />
					<Add option="-leagle_a5-static" />
					<Add option="-leagle-static" />
					<Add option="-lallegro_monolith-static" />
					<Add option="-ljpeg" />
					<Add option="-ldumb" />
					<Add option="-lFLAC" />
					<Add option="-lvorbisfile" />
					<Add option="-lvorbis" />
					<Add option="-lfreetype" />
					<Add option="-logg" />
					<Add option="-lpng16" />
					<Add option="-lzlibstatic" />
					<Add option="-lgdiplus" />
					<Add option="-luuid" />
					<Add option="-lkernel32" />
					<Add option="-lwinmm" />
					<Add option="-lpsapi" />
					<Add option="-ldsound" />
					<Add option="-ld3dx9" />
					<Add option="-lopengl32" />
					<Add option="-lglu32" />
					<Add option="-luser32" />
					<Add option="-lcomdlg32" />
					<Add option="-lgdi32" />
					<Add option="-lshell32" />
					<Add option="-lole32" />
					<Add option="-ladvapi32" />
					<Add option="-lws2_32" />
					<Add option="-lshlwapi" />
					<Add option="-lgcc_eh" />
				</Linker>
			</Target>
			<Target title="StaticProfiling">
				<Option output="cbbuild/bin/Libtest-profile-static" prefix_auto="1" extension_auto="1" />
				<Option working_dir="cbbuild/bin" />
				<Option object_output="cbbuild/obj/Static/Profiling" />
				<Option external_deps="cbbuild/lib/libeagle_a5_profile-static.a;cbbuild/lib/libeagle_profile-static.a;" />
				<Option type="1" />
				<Option compiler="gcc" />
				<Compiler>
					<Add option="-O2" />
					<Add option="-DALLEGRO_STATICLINK" />
				</Compiler>
				<Linker>
					<Add option="-static-libstdc++" />
					<Add option="-static-libgcc" />
					<Add option="-static" />
					<Add option="-leagle_a5_profile-static" />
					<Add option="-leagle_profile-static" />
					<Add option="-lallegro_monolith-static" />
					<Add option="-ljpeg" />
					<Add option="-ldumb" />
					<Add option="-lFLAC" />
					<Add option="-lvorbisfile" />
					<Add option="-lvorbis" />
					<Add option="-lfreetype" />
					<Add option="-logg" />
					<Add option="-lpng16" />
					<Add option="-lzlibstatic" />
					<Add option="-lgdiplus" />
					<Add option="-luuid" />
					<Add option="-lkernel32" />
					<Add option="-lwinmm" />
					<Add option="-lpsapi" />
					<Add option="-ldsound" />
					<Add option="-ld3dx9" />
					<Add option="-lopengl32" />
					<Add option="-lglu32" />
					<Add option="-luser32" />
					<Add option="-lcomdlg32" />
					<Add option="-lgdi32" />
					<Add option="-lshell32" />
					<Add option="-lole32" />
					<Add option="-ladvapi32" />
					<Add option="-lws2_32" />
					<Add option="-lshlwapi" />
					<Add option="-lgcc_eh" />
					<Add option="-lm" />
					<Add option="-pg" />
				</Linker>
			</Target>
		</Build>
		<VirtualTargets>
			<Add alias="All" targets="Debug;Release;Profiling;StaticDebug;StaticRelease;StaticProfiling;" />
			<Add alias="AllDynamic" targets="Debug;Release;Profiling;" />
			<Add alias="AllStatic" targets="StaticDebug;StaticRelease;StaticProfiling;" />
		</VirtualTargets>
		<Compiler>
			<Add option="-Wshadow" />
			<Add option="-Wall" />
			<Add option="-std=c++11" />
			<Add option="-DNOGDI" />
			<Add directory="include" />
			<Add directory="Allegro5/include" />
		</Compiler>
		<Linker>
			<Add directory="cbbuild/lib" />
			<Add directory="Allegro5/lib" />
		</Linker>
		<Extensions>
			<code_completion />
			<debugger />
			<envvars />
		</Extensions>
	</Project>
</CodeBlocks_project_file>
