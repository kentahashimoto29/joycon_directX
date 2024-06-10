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
 184;
 -2.96164;11.29671;-2.86270;,
 -1.48082;11.09197;-2.86270;,
 -1.90973;9.90398;-3.54261;,
 -3.81946;9.90398;-2.67952;,
 0.00000;10.68788;-3.18487;,
 0.00000;9.90398;-3.54261;,
 1.48082;11.09197;-2.86270;,
 1.90973;9.90398;-3.54261;,
 2.96164;11.29671;-2.86270;,
 3.81946;9.90398;-2.67952;,
 -2.36653;8.51124;-3.84147;,
 -4.73307;8.51124;-2.90668;,
 0.00000;8.51124;-3.84147;,
 2.36653;8.51124;-3.84147;,
 4.73307;8.51124;-2.90668;,
 -2.36653;5.91504;-3.84147;,
 -4.73307;5.91504;-2.90668;,
 0.00000;5.91504;-3.84147;,
 2.36653;5.91504;-3.84147;,
 4.73307;5.91504;-2.90668;,
 -2.26266;3.92058;-3.59684;,
 -4.52531;3.92058;-2.72074;,
 0.00000;3.92058;-3.59684;,
 2.26266;3.92058;-3.59684;,
 4.52531;3.92058;-2.72074;,
 -2.12061;1.92115;-3.01191;,
 -4.24121;1.92115;-2.27614;,
 0.00000;1.92115;-3.01191;,
 2.12061;1.92115;-3.01191;,
 4.24121;1.92115;-2.27614;,
 -1.86269;-0.06340;-2.38317;,
 -3.72539;-0.06340;-2.38317;,
 0.00000;-0.06340;-2.38317;,
 1.86269;-0.06340;-2.38317;,
 3.72539;-0.06340;-1.79824;,
 2.96164;11.29671;-2.86270;,
 2.96164;11.29671;-1.40387;,
 3.81946;9.90398;-1.31228;,
 3.81946;9.90398;-2.67952;,
 2.96164;11.29671;0.05496;,
 3.81946;9.90398;0.05496;,
 2.96164;11.29671;1.51379;,
 3.81946;9.90398;1.42220;,
 2.96164;11.29671;2.97262;,
 3.81946;9.90398;2.78945;,
 4.73307;8.51124;-1.42586;,
 4.73307;8.51124;-2.90668;,
 4.73307;8.51124;0.05496;,
 4.73307;8.51124;1.53578;,
 4.73307;8.51124;3.01661;,
 4.73307;5.91504;-1.42586;,
 4.73307;5.91504;-2.90668;,
 4.73307;5.91504;0.05496;,
 4.73307;5.91504;1.53578;,
 4.52531;5.91504;3.01661;,
 4.52531;3.92058;-1.33289;,
 4.52531;3.92058;-2.72074;,
 4.52531;3.92058;0.05496;,
 4.52531;3.92058;1.44282;,
 4.52531;3.92058;2.83067;,
 4.24121;1.92115;-1.11059;,
 4.24121;1.92115;-2.27614;,
 4.24121;1.92115;0.05496;,
 4.24121;1.92115;1.22052;,
 4.24121;1.92115;2.60440;,
 3.72539;-0.06340;-0.87164;,
 3.72539;-0.06340;-1.79824;,
 3.72539;-0.06340;0.05496;,
 3.72539;-0.06340;0.98156;,
 3.72539;-0.06340;2.37570;,
 2.96164;11.29671;2.97262;,
 1.48082;11.09197;2.97262;,
 1.90973;9.90398;3.65253;,
 3.81946;9.90398;2.78945;,
 0.00000;10.68788;3.29479;,
 0.00000;9.90398;3.65253;,
 -1.48082;11.09197;2.97262;,
 -1.90973;9.90398;3.65253;,
 -2.96164;11.29671;2.97262;,
 -3.81946;9.90398;2.78945;,
 2.36653;8.51124;3.95139;,
 4.73307;8.51124;3.01661;,
 0.00000;8.51124;3.95139;,
 -2.36653;8.51124;3.95139;,
 -4.73307;8.51124;3.01661;,
 2.26266;5.91504;3.95139;,
 4.52531;5.91504;3.01661;,
 0.00000;5.91504;3.95139;,
 -2.26266;5.91504;3.95139;,
 -4.52531;5.91504;3.01661;,
 2.26266;3.92058;3.70677;,
 4.52531;3.92058;2.83067;,
 0.00000;3.92058;3.70677;,
 -2.26266;3.92058;3.70677;,
 -4.52531;3.92058;2.83067;,
 2.12061;1.92115;3.40908;,
 4.24121;1.92115;2.60440;,
 0.00000;1.92115;3.40908;,
 -2.12061;1.92115;3.40908;,
 -4.24121;1.92115;2.60440;,
 1.86269;-0.06340;3.10819;,
 3.72539;-0.06340;2.37570;,
 0.00000;-0.06340;3.10819;,
 -1.86269;-0.06340;3.10819;,
 -3.72539;-0.06340;3.10819;,
 -2.96164;11.29671;2.97262;,
 -2.96164;11.29671;1.51379;,
 -3.81946;9.90398;1.42220;,
 -3.81946;9.90398;2.78945;,
 -2.96164;11.29671;0.05496;,
 -3.81946;9.90398;0.05496;,
 -2.96164;11.29671;-1.40387;,
 -3.81946;9.90398;-1.31228;,
 -2.96164;11.29671;-2.86270;,
 -3.81946;9.90398;-2.67952;,
 -4.73307;8.51124;1.53578;,
 -4.73307;8.51124;3.01661;,
 -4.73307;8.51124;0.05496;,
 -4.73307;8.51124;-1.42586;,
 -4.73307;8.51124;-2.90668;,
 -4.73307;5.91504;1.53578;,
 -4.52531;5.91504;3.01661;,
 -4.73307;5.91504;0.05496;,
 -4.73307;5.91504;-1.42586;,
 -4.73307;5.91504;-2.90668;,
 -4.52531;3.92058;1.44282;,
 -4.52531;3.92058;2.83067;,
 -4.52531;3.92058;0.05496;,
 -4.52531;3.92058;-1.33289;,
 -4.52531;3.92058;-2.72074;,
 -4.24121;1.92115;1.22052;,
 -4.24121;1.92115;2.60440;,
 -4.24121;1.92115;0.05496;,
 -4.24121;1.92115;-1.11059;,
 -4.24121;1.92115;-2.27614;,
 -3.72539;-0.06340;1.27403;,
 -3.72539;-0.06340;3.10819;,
 -3.72539;-0.06340;0.05496;,
 -3.72539;-0.06340;-1.16410;,
 -3.72539;-0.06340;-2.38317;,
 -1.48082;11.09197;2.97262;,
 -1.48082;11.09197;1.51379;,
 -2.96164;11.29671;1.51379;,
 0.00000;10.68788;1.67488;,
 1.48082;11.09197;2.97262;,
 1.48082;11.09197;1.51379;,
 2.96164;11.29671;1.51379;,
 -1.48082;11.09197;0.05496;,
 -2.96164;11.29671;0.05496;,
 0.00000;10.68788;0.05496;,
 1.48082;11.09197;0.05496;,
 2.96164;11.29671;0.05496;,
 -1.48082;11.09197;-1.40387;,
 -2.96164;11.29671;-1.40387;,
 0.00000;10.68788;-1.56495;,
 1.48082;11.09197;-1.40387;,
 2.96164;11.29671;-1.40387;,
 -1.48082;11.09197;-2.86270;,
 -2.96164;11.29671;-2.86270;,
 0.00000;10.68788;-3.18487;,
 1.48082;11.09197;-2.86270;,
 2.96164;11.29671;-2.86270;,
 -3.72539;-0.06340;1.27403;,
 -1.86269;-0.06340;1.27403;,
 -1.86269;-0.06340;3.10819;,
 0.00000;-0.06340;1.27403;,
 1.86269;-0.06340;1.27403;,
 1.86269;-0.06340;3.10819;,
 3.72539;-0.06340;0.98156;,
 -3.72539;-0.06340;0.05496;,
 -1.86269;-0.06340;0.05496;,
 0.00000;-0.06340;0.05496;,
 1.86269;-0.06340;0.05496;,
 3.72539;-0.06340;0.05496;,
 -3.72539;-0.06340;-1.16410;,
 -1.86269;-0.06340;-1.16410;,
 0.00000;-0.06340;-1.16410;,
 1.86269;-0.06340;-1.16410;,
 3.72539;-0.06340;-0.87164;,
 -3.72539;-0.06340;-2.38317;,
 -1.86269;-0.06340;-2.38317;,
 0.00000;-0.06340;-2.38317;,
 1.86269;-0.06340;-2.38317;,
 3.72539;-0.06340;-1.79824;;
 
 128;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;3,2,10,11;,
 4;2,5,12,10;,
 4;5,7,13,12;,
 4;7,9,14,13;,
 4;11,10,15,16;,
 4;10,12,17,15;,
 4;12,13,18,17;,
 4;13,14,19,18;,
 4;16,15,20,21;,
 4;15,17,22,20;,
 4;17,18,23,22;,
 4;18,19,24,23;,
 4;21,20,25,26;,
 4;20,22,27,25;,
 4;22,23,28,27;,
 4;23,24,29,28;,
 4;26,25,30,31;,
 4;25,27,32,30;,
 4;27,28,33,32;,
 4;28,29,34,33;,
 4;35,36,37,38;,
 4;36,39,40,37;,
 4;39,41,42,40;,
 4;41,43,44,42;,
 4;38,37,45,46;,
 4;37,40,47,45;,
 4;40,42,48,47;,
 4;42,44,49,48;,
 4;46,45,50,51;,
 4;45,47,52,50;,
 4;47,48,53,52;,
 4;48,49,54,53;,
 4;51,50,55,56;,
 4;50,52,57,55;,
 4;52,53,58,57;,
 4;53,54,59,58;,
 4;56,55,60,61;,
 4;55,57,62,60;,
 4;57,58,63,62;,
 4;58,59,64,63;,
 4;61,60,65,66;,
 4;60,62,67,65;,
 4;62,63,68,67;,
 4;63,64,69,68;,
 4;70,71,72,73;,
 4;71,74,75,72;,
 4;74,76,77,75;,
 4;76,78,79,77;,
 4;73,72,80,81;,
 4;72,75,82,80;,
 4;75,77,83,82;,
 4;77,79,84,83;,
 4;81,80,85,86;,
 4;80,82,87,85;,
 4;82,83,88,87;,
 4;83,84,89,88;,
 4;86,85,90,91;,
 4;85,87,92,90;,
 4;87,88,93,92;,
 4;88,89,94,93;,
 4;91,90,95,96;,
 4;90,92,97,95;,
 4;92,93,98,97;,
 4;93,94,99,98;,
 4;96,95,100,101;,
 4;95,97,102,100;,
 4;97,98,103,102;,
 4;98,99,104,103;,
 4;105,106,107,108;,
 4;106,109,110,107;,
 4;109,111,112,110;,
 4;111,113,114,112;,
 4;108,107,115,116;,
 4;107,110,117,115;,
 4;110,112,118,117;,
 4;112,114,119,118;,
 4;116,115,120,121;,
 4;115,117,122,120;,
 4;117,118,123,122;,
 4;118,119,124,123;,
 4;121,120,125,126;,
 4;120,122,127,125;,
 4;122,123,128,127;,
 4;123,124,129,128;,
 4;126,125,130,131;,
 4;125,127,132,130;,
 4;127,128,133,132;,
 4;128,129,134,133;,
 4;131,130,135,136;,
 4;130,132,137,135;,
 4;132,133,138,137;,
 4;133,134,139,138;,
 4;105,140,141,142;,
 4;140,74,143,141;,
 4;74,144,145,143;,
 4;144,43,146,145;,
 4;142,141,147,148;,
 4;141,143,149,147;,
 4;143,145,150,149;,
 4;145,146,151,150;,
 4;148,147,152,153;,
 4;147,149,154,152;,
 4;149,150,155,154;,
 4;150,151,156,155;,
 4;153,152,157,158;,
 4;152,154,159,157;,
 4;154,155,160,159;,
 4;155,156,161,160;,
 4;162,163,164,136;,
 4;163,165,102,164;,
 4;165,166,167,102;,
 4;166,168,69,167;,
 4;169,170,163,162;,
 4;170,171,165,163;,
 4;171,172,166,165;,
 4;172,173,168,166;,
 4;174,175,170,169;,
 4;175,176,171,170;,
 4;176,177,172,171;,
 4;177,178,173,172;,
 4;179,180,175,174;,
 4;180,181,176,175;,
 4;181,182,177,176;,
 4;182,183,178,177;;
 
 MeshMaterialList {
  1;
  128;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  121;
  -0.178967;0.319158;-0.930650;,
  -0.102107;0.393357;-0.913698;,
  0.000000;0.463087;-0.886313;,
  0.102107;0.393357;-0.913698;,
  -0.274215;0.331197;-0.902837;,
  -0.144727;0.338319;-0.929836;,
  0.000000;0.339521;-0.940599;,
  0.144727;0.338319;-0.929836;,
  -0.372668;0.172565;-0.911779;,
  -0.189023;0.141551;-0.971717;,
  0.000000;0.105493;-0.994420;,
  0.189023;0.141551;-0.971717;,
  -0.365061;-0.064141;-0.928772;,
  -0.185747;-0.063671;-0.980533;,
  0.000000;-0.060985;-0.998139;,
  0.185747;-0.063672;-0.980533;,
  -0.347481;-0.198949;-0.916338;,
  -0.176573;-0.203638;-0.962992;,
  0.000000;-0.201928;-0.979400;,
  0.176573;-0.203638;-0.962992;,
  -0.251028;-0.221558;-0.942283;,
  -0.126276;-0.258789;-0.957644;,
  0.000000;-0.291419;-0.956595;,
  0.159622;-0.294440;-0.942245;,
  -0.167596;-0.172021;-0.970732;,
  -0.084277;-0.238375;-0.967510;,
  0.000000;-0.302022;-0.953301;,
  0.150865;-0.309953;-0.938706;,
  0.178967;0.319158;-0.930650;,
  0.851452;0.524432;0.000000;,
  0.274215;0.331197;-0.902837;,
  0.843888;0.536520;0.000000;,
  0.372668;0.172565;-0.911779;,
  0.958163;0.286224;0.000000;,
  0.960736;0.276867;0.018192;,
  0.365061;-0.064141;-0.928772;,
  0.998654;-0.051874;-0.000000;,
  0.998654;-0.051874;-0.000000;,
  0.998116;-0.050495;0.034840;,
  0.347481;-0.198949;-0.916338;,
  0.992510;-0.122164;-0.000000;,
  0.993670;-0.110976;0.017444;,
  0.315188;-0.289892;-0.903670;,
  0.980517;-0.196433;-0.000000;,
  0.980517;-0.196433;-0.000000;,
  0.298255;-0.310744;-0.902487;,
  0.967842;-0.251560;-0.000000;,
  0.851452;0.524432;0.000000;,
  0.102105;0.393358;0.913698;,
  0.000000;0.463087;0.886313;,
  -0.102105;0.393358;0.913698;,
  0.144725;0.338319;0.929836;,
  0.000000;0.339521;0.940599;,
  -0.144725;0.338319;0.929836;,
  0.962914;0.267329;0.036498;,
  0.191097;0.135968;0.972108;,
  0.000000;0.105493;0.994420;,
  -0.191097;0.135968;0.972108;,
  0.996369;-0.049057;0.069593;,
  0.189721;-0.062134;0.979870;,
  0.000000;-0.060982;0.998139;,
  -0.189721;-0.062134;0.979870;,
  0.994403;-0.099725;0.034903;,
  0.183945;-0.134133;0.973742;,
  0.000000;-0.134512;0.990912;,
  -0.183945;-0.134133;0.973742;,
  0.980517;-0.196433;-0.000000;,
  0.179763;-0.164340;0.969885;,
  0.000000;-0.148584;0.988900;,
  -0.135263;-0.116298;0.983961;,
  0.967842;-0.251560;-0.000000;,
  0.179879;-0.173206;0.968320;,
  0.000000;-0.149903;0.988701;,
  -0.090831;-0.077088;0.992878;,
  -0.178965;0.319159;0.930650;,
  -0.851452;0.524432;0.000000;,
  -0.274212;0.331198;0.902838;,
  -0.843888;0.536520;0.000000;,
  -0.377039;0.161547;0.912000;,
  -0.960736;0.276867;0.018192;,
  -0.958163;0.286224;0.000000;,
  -0.372350;-0.061031;0.926084;,
  -0.998116;-0.050495;0.034840;,
  -0.998654;-0.051874;0.000000;,
  -0.998654;-0.051874;0.000000;,
  -0.361678;-0.129174;0.923311;,
  -0.993670;-0.110976;0.017444;,
  -0.992510;-0.122164;0.000000;,
  -0.268867;-0.081499;0.959723;,
  -0.980517;-0.196433;0.000000;,
  -0.980517;-0.196433;0.000000;,
  -0.180429;-0.003226;0.983583;,
  -0.967842;-0.251560;0.000000;,
  -0.967842;-0.251560;0.000000;,
  -0.967842;-0.251560;0.000000;,
  0.200525;0.979689;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.200525;0.979689;0.000000;,
  0.200525;0.979689;0.000000;,
  -0.200525;0.979689;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.992510;-0.122164;-0.000000;,
  0.178965;0.319159;0.930650;,
  0.274212;0.331198;0.902838;,
  0.377039;0.161547;0.912000;,
  0.372350;-0.061031;0.926084;,
  0.361678;-0.129174;0.923311;,
  0.353630;-0.174687;0.918929;,
  0.353765;-0.190739;0.915680;,
  -0.851452;0.524432;0.000000;,
  -0.962914;0.267329;0.036498;,
  -0.996369;-0.049057;0.069593;,
  -0.994403;-0.099725;0.034903;,
  -0.992510;-0.122164;0.000000;,
  -0.980517;-0.196433;0.000000;,
  -0.967842;-0.251560;0.000000;,
  0.136958;0.990577;0.000000;,
  -0.136958;0.990577;0.000000;,
  0.136958;0.990577;0.000000;,
  -0.136958;0.990577;0.000000;,
  0.200525;0.979689;0.000000;;
  128;
  4;0,1,5,4;,
  4;1,2,6,5;,
  4;2,3,7,6;,
  4;3,28,30,7;,
  4;4,5,9,8;,
  4;5,6,10,9;,
  4;6,7,11,10;,
  4;7,30,32,11;,
  4;8,9,13,12;,
  4;9,10,14,13;,
  4;10,11,15,14;,
  4;11,32,35,15;,
  4;12,13,17,16;,
  4;13,14,18,17;,
  4;14,15,19,18;,
  4;15,35,39,19;,
  4;16,17,21,20;,
  4;17,18,22,21;,
  4;18,19,23,22;,
  4;19,39,42,23;,
  4;20,21,25,24;,
  4;21,22,26,25;,
  4;22,23,27,26;,
  4;23,42,45,27;,
  4;47,29,31,31;,
  4;29,29,31,31;,
  4;29,29,31,31;,
  4;29,47,31,31;,
  4;31,31,33,33;,
  4;31,31,33,33;,
  4;31,31,34,33;,
  4;31,31,54,34;,
  4;33,33,36,36;,
  4;33,33,37,36;,
  4;33,34,38,37;,
  4;34,54,58,38;,
  4;36,36,40,101;,
  4;36,37,40,40;,
  4;37,38,41,40;,
  4;38,58,62,41;,
  4;101,40,43,43;,
  4;40,40,44,43;,
  4;40,41,43,44;,
  4;41,62,66,43;,
  4;43,43,46,70;,
  4;43,44,46,46;,
  4;44,43,46,46;,
  4;43,66,70,46;,
  4;102,48,51,103;,
  4;48,49,52,51;,
  4;49,50,53,52;,
  4;50,74,76,53;,
  4;103,51,55,104;,
  4;51,52,56,55;,
  4;52,53,57,56;,
  4;53,76,78,57;,
  4;104,55,59,105;,
  4;55,56,60,59;,
  4;56,57,61,60;,
  4;57,78,81,61;,
  4;105,59,63,106;,
  4;59,60,64,63;,
  4;60,61,65,64;,
  4;61,81,85,65;,
  4;106,63,67,107;,
  4;63,64,68,67;,
  4;64,65,69,68;,
  4;65,85,88,69;,
  4;107,67,71,108;,
  4;67,68,72,71;,
  4;68,69,73,72;,
  4;69,88,91,73;,
  4;109,75,77,77;,
  4;75,75,77,77;,
  4;75,75,77,77;,
  4;75,109,77,77;,
  4;77,77,79,110;,
  4;77,77,80,79;,
  4;77,77,80,80;,
  4;77,77,80,80;,
  4;110,79,82,111;,
  4;79,80,83,82;,
  4;80,80,84,83;,
  4;80,80,83,84;,
  4;111,82,86,112;,
  4;82,83,87,86;,
  4;83,84,87,87;,
  4;84,83,113,87;,
  4;112,86,89,114;,
  4;86,87,89,89;,
  4;87,87,90,89;,
  4;87,113,90,90;,
  4;114,89,92,115;,
  4;89,89,93,92;,
  4;89,90,94,93;,
  4;90,90,115,94;,
  4;116,95,95,116;,
  4;95,96,96,95;,
  4;96,97,97,96;,
  4;97,117,117,97;,
  4;116,95,95,118;,
  4;95,96,96,95;,
  4;96,97,97,96;,
  4;97,117,119,97;,
  4;118,95,98,118;,
  4;95,96,96,98;,
  4;96,97,99,96;,
  4;97,119,119,99;,
  4;118,98,120,116;,
  4;98,96,96,120;,
  4;96,99,99,96;,
  4;99,119,117,99;,
  4;100,100,100,100;,
  4;100,100,100,100;,
  4;100,100,100,100;,
  4;100,100,100,100;,
  4;100,100,100,100;,
  4;100,100,100,100;,
  4;100,100,100,100;,
  4;100,100,100,100;,
  4;100,100,100,100;,
  4;100,100,100,100;,
  4;100,100,100,100;,
  4;100,100,100,100;,
  4;100,100,100,100;,
  4;100,100,100,100;,
  4;100,100,100,100;,
  4;100,100,100,100;;
 }
 MeshTextureCoords {
  184;
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.166670;,
  0.000000;0.166670;,
  0.500000;0.000000;,
  0.500000;0.166670;,
  0.750000;0.000000;,
  0.750000;0.166670;,
  1.000000;0.000000;,
  1.000000;0.166670;,
  0.250000;0.333330;,
  0.000000;0.333330;,
  0.500000;0.333330;,
  0.750000;0.333330;,
  1.000000;0.333330;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.250000;0.666670;,
  0.000000;0.666670;,
  0.500000;0.666670;,
  0.750000;0.666670;,
  1.000000;0.666670;,
  0.250000;0.833330;,
  0.000000;0.833330;,
  0.500000;0.833330;,
  0.750000;0.833330;,
  1.000000;0.833330;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.166670;,
  0.000000;0.166670;,
  0.500000;0.000000;,
  0.500000;0.166670;,
  0.750000;0.000000;,
  0.750000;0.166670;,
  1.000000;0.000000;,
  1.000000;0.166670;,
  0.250000;0.333330;,
  0.000000;0.333330;,
  0.500000;0.333330;,
  0.750000;0.333330;,
  1.000000;0.333330;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.250000;0.666670;,
  0.000000;0.666670;,
  0.500000;0.666670;,
  0.750000;0.666670;,
  1.000000;0.666670;,
  0.250000;0.833330;,
  0.000000;0.833330;,
  0.500000;0.833330;,
  0.750000;0.833330;,
  1.000000;0.833330;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.166670;,
  0.000000;0.166670;,
  0.500000;0.000000;,
  0.500000;0.166670;,
  0.750000;0.000000;,
  0.750000;0.166670;,
  1.000000;0.000000;,
  1.000000;0.166670;,
  0.250000;0.333330;,
  0.000000;0.333330;,
  0.500000;0.333330;,
  0.750000;0.333330;,
  1.000000;0.333330;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.250000;0.666670;,
  0.000000;0.666670;,
  0.500000;0.666670;,
  0.750000;0.666670;,
  1.000000;0.666670;,
  0.250000;0.833330;,
  0.000000;0.833330;,
  0.500000;0.833330;,
  0.750000;0.833330;,
  1.000000;0.833330;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.166670;,
  0.000000;0.166670;,
  0.500000;0.000000;,
  0.500000;0.166670;,
  0.750000;0.000000;,
  0.750000;0.166670;,
  1.000000;0.000000;,
  1.000000;0.166670;,
  0.250000;0.333330;,
  0.000000;0.333330;,
  0.500000;0.333330;,
  0.750000;0.333330;,
  1.000000;0.333330;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.250000;0.666670;,
  0.000000;0.666670;,
  0.500000;0.666670;,
  0.750000;0.666670;,
  1.000000;0.666670;,
  0.250000;0.833330;,
  0.000000;0.833330;,
  0.500000;0.833330;,
  0.750000;0.833330;,
  1.000000;0.833330;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  0.500000;0.250000;,
  0.750000;0.000000;,
  0.750000;0.250000;,
  1.000000;0.250000;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.250000;0.750000;,
  0.000000;0.750000;,
  0.500000;0.750000;,
  0.750000;0.750000;,
  1.000000;0.750000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.250000;1.000000;,
  0.500000;0.750000;,
  0.750000;0.750000;,
  0.750000;1.000000;,
  1.000000;0.750000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.500000;0.250000;,
  0.750000;0.250000;,
  1.000000;0.250000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;;
 }
}