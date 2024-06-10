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
 44;
 -16.06801;19.22627;17.70731;,
 -12.47088;28.60090;17.70731;,
 -12.47088;-1.08614;17.70731;,
 -14.95783;-1.08614;17.70731;,
 -8.87375;19.22627;17.70731;,
 -9.98392;-1.08614;17.70731;,
 -8.87375;19.22627;19.48759;,
 -9.98392;-1.08614;19.48759;,
 -12.47088;28.60090;19.48759;,
 -12.47088;-1.08614;19.48759;,
 -16.06801;19.22627;19.48759;,
 -14.95783;-1.08614;19.48759;,
 -3.43181;19.22627;17.70731;,
 0.16532;28.60090;17.70731;,
 0.16532;-1.08614;17.70731;,
 -2.32163;-1.08614;17.70731;,
 3.76245;19.22627;17.70731;,
 2.65228;-1.08614;17.70731;,
 3.76245;19.22627;19.48759;,
 2.65228;-1.08614;19.48759;,
 0.16532;28.60090;19.48759;,
 0.16532;-1.08614;19.48759;,
 -3.43181;19.22627;19.48759;,
 -2.32163;-1.08614;19.48759;,
 9.40808;19.22627;17.70731;,
 13.00521;28.60090;17.70731;,
 13.00521;-1.08614;17.70731;,
 10.51826;-1.08614;17.70731;,
 16.60234;19.22627;17.70731;,
 15.49216;-1.08614;17.70731;,
 16.60234;19.22627;19.48759;,
 15.49216;-1.08614;19.48759;,
 13.00521;28.60090;19.48759;,
 13.00521;-1.08614;19.48759;,
 9.40808;19.22627;19.48759;,
 10.51826;-1.08614;19.48759;,
 -20.00000;16.61951;17.14975;,
 20.00000;16.61951;17.14975;,
 20.00000;11.28551;17.14975;,
 -20.00000;11.28551;17.14975;,
 20.00000;16.61951;19.93875;,
 20.00000;11.28551;19.93875;,
 -20.00000;16.61951;19.93875;,
 -20.00000;11.28551;19.93875;;
 
 36;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,0,3,11;,
 4;10,8,1,0;,
 4;8,6,4,1;,
 4;3,2,9,11;,
 4;2,5,7,9;,
 4;12,13,14,15;,
 4;13,16,17,14;,
 4;16,18,19,17;,
 4;18,20,21,19;,
 4;20,22,23,21;,
 4;22,12,15,23;,
 4;22,20,13,12;,
 4;20,18,16,13;,
 4;15,14,21,23;,
 4;14,17,19,21;,
 4;24,25,26,27;,
 4;25,28,29,26;,
 4;28,30,31,29;,
 4;30,32,33,31;,
 4;32,34,35,33;,
 4;34,24,27,35;,
 4;34,32,25,24;,
 4;32,30,28,25;,
 4;27,26,33,35;,
 4;26,29,31,33;,
 4;36,37,38,39;,
 4;37,40,41,38;,
 4;40,42,43,41;,
 4;42,36,39,43;,
 4;42,40,37,36;,
 4;39,38,41,43;;
 
 MeshMaterialList {
  1;
  36;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.489412;0.188235;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
}
