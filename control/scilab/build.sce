// scilab-cli -f build.sci

//ilib_for_link('lim_int','test_xcos_interf.c',[],'c','Makelib','loader.sce','','','-g');
ilib_for_link('lim_int','test_xcos_interf.cpp',['test_link.o'],'c','','loader.sce','','','-g');

exit;
