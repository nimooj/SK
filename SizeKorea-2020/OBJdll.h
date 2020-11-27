#ifndef __OBJ_DLL__
#define __OBJ_DLL__

#ifndef _OBJLIB
#define _OBJLIB __declspec(dllexport)
#else
#define _OBJLIB __declspec(dllimport)
#endif /* _OBJLIB */

//#ifdef __cplusplus
extern "C" {
//#endif /* __cplusplus */

_OBJLIB bool _stdcall ImportOBJ(char	*FileName,
                        int *nNode,
                        float **ppNodes,
                        float **ppColor,
                        float **ppNormal,
                        int *nElement,
                        int **ppElements);
	// pfFaceVertexCoords �� Array Size : iFaceCount * 3 * 3   (Face�� Vertex 3��, Vertex�� x/y/z 3��)
	_OBJLIB int _stdcall OutputMesh();

//#ifdef __cplusplus
}
//#endif /* __cplusplus */

#endif /* __OBJ_DLL__ */