xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 28;
 1.89034;-1.76897;-8.97522;,
 2.83143;-3.00626;-9.24528;,
 -2.83184;-3.00626;-9.24528;,
 -1.89075;-1.76897;-8.97522;,
 2.83143;-4.24355;-9.09183;,
 -2.83184;-4.24355;-9.09183;,
 -1.89075;-1.76897;-8.97522;,
 -2.83184;-3.00626;-9.24528;,
 -1.61993;-2.42001;2.78120;,
 -1.61993;-0.56682;2.12369;,
 -2.83184;-4.24355;-9.09183;,
 -1.61993;-4.27322;2.12369;,
 -1.61993;-0.56682;2.12369;,
 -1.61993;-2.42001;2.78120;,
 1.61951;-2.42001;2.78120;,
 1.61951;-0.56682;2.12369;,
 -1.61993;-4.27322;2.12369;,
 1.61951;-4.27322;2.12369;,
 1.61951;-0.56682;2.12369;,
 1.61951;-2.42001;2.78120;,
 2.83143;-3.00626;-9.24528;,
 1.89034;-1.76897;-8.97522;,
 1.61951;-4.27322;2.12369;,
 2.83143;-4.24355;-9.09183;,
 1.89034;-1.76897;-8.97522;,
 -1.89075;-1.76897;-8.97522;,
 2.83143;-4.24355;-9.09183;,
 -2.83184;-4.24355;-9.09183;;
 
 10;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;6,7,8,9;,
 4;7,10,11,8;,
 4;12,13,14,15;,
 4;13,16,17,14;,
 4;18,19,20,21;,
 4;19,22,23,20;,
 4;18,24,25,9;,
 4;26,22,11,27;;
 
 MeshMaterialList {
  1;
  10;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  14;
  0.000000;0.213247;-0.976998;,
  0.000000;0.045737;-0.998953;,
  0.000000;-0.123078;-0.992397;,
  -0.947027;0.318721;0.039466;,
  -0.985057;0.155916;0.073163;,
  -0.994435;-0.011424;0.104732;,
  0.000000;0.334377;0.942440;,
  0.000000;0.000002;1.000000;,
  0.000000;-0.334373;0.942441;,
  0.947027;0.318721;0.039466;,
  0.985057;0.155916;0.073164;,
  0.994435;-0.011424;0.104733;,
  0.000000;0.994185;-0.107683;,
  0.000000;-0.999997;-0.002645;;
  10;
  4;0,1,1,0;,
  4;1,2,2,1;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;9,10,10,9;,
  4;10,11,11,10;,
  4;12,12,12,12;,
  4;13,13,13,13;;
 }
 MeshTextureCoords {
  28;
  0.000000;0.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
