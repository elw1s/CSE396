import 'package:flutter/material.dart';
import 'dart:typed_data';
import 'dart:io';

import 'package:image_picker/image_picker.dart';
import 'package:flutter/foundation.dart';

class HomeScreen extends StatefulWidget {
  @override
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  File? _image;
  bool draw = false;
  bool isDrawing = false;

  @override
  void initState(){
    super.initState();
    connectionThread();
  }

  void connectionThread() async{
    try {
        final Directory tempDir = Directory.systemTemp;
        String imagePath = tempDir.path + '/' + "image.jpg";
        final chunkSize = 4095;
        Uint8List imageBytes = Uint8List(0);
        print("test1");


        final socket = await Socket.connect("192.168.31.25", 8080);

        socket.write("0Connection Established");

        int i = 0;
        Uint8List chunkWithPrefix = Uint8List.fromList([]);
        int calculatedValue= 0;
        int totalLineNumberForAnImage = 0;
        socket.listen(
              (Uint8List data) {
                print("---------------------------------------\n");
                print('Sunucudan gelen veri: ${String.fromCharCodes(data)}');
              final serverResponse = Uint8List.fromList(data)[0] - 48;
              print("gelen komut -> $serverResponse");

              if (serverResponse == 0) {

                if(draw){
                  final imageFile = File(imagePath);
                  imageBytes = imageFile.readAsBytesSync();
                  print("Image length: ${imageBytes.length}");
                  print("Image length in bytes: ${imageBytes.lengthInBytes}");
                  print("Image size in KB: ${imageBytes.lengthInBytes / 1024}");
                  print("Image size in MB: ${(imageBytes.lengthInBytes / 1024) / 1024}");
                  if (i < imageBytes.length) {
                    final end = (i + chunkSize < imageBytes.length)
                        ? i + chunkSize
                        : imageBytes.length;
                    final chunk = imageBytes.sublist(i, end);

                    // Add '1' at the beginning of each chunk, '2' for the last chunk
                    final prefix = (end == imageBytes.length)
                        ? Uint8List.fromList([2])
                        : Uint8List.fromList([1]);
                    chunkWithPrefix = Uint8List.fromList(prefix + chunk);

                    socket.add(chunkWithPrefix);

                    if(prefix.elementAt(0) == 2){
                      draw = false;
                    }

                    print('Sent chunk ${i ~/ chunkSize + 1}');
                    i += chunkSize;
                  }
                  isDrawing = true;
                }
                else{
                  socket.write("0Connection Established");
                }
              }
              else if (serverResponse == 1) {
                if (i < imageBytes.length) {
                  final end = (i + chunkSize < imageBytes.length)
                      ? i + chunkSize
                      : imageBytes.length;
                  final chunk = imageBytes.sublist(i, end);

                  // Add '1' at the beginning of each chunk, '2' for the last chunk
                  final prefix = (end == imageBytes.length)
                      ? Uint8List.fromList([2])
                      : Uint8List.fromList([1]);
                  chunkWithPrefix = Uint8List.fromList(prefix + chunk);

                  socket.add(chunkWithPrefix);

                  if(prefix.elementAt(0) == 2){
                    draw = false;
                  }
                  print('Sent chunk ${i ~/ chunkSize + 1} with ${prefix.elementAt(0)} as prefix');
                  i += chunkSize;
                }
              }
              else if (serverResponse == 2) {
                socket.write("6Connection Established");
              }
              else if (serverResponse == 3) {
                final serverResponseString = String.fromCharCodes(data);
                totalLineNumberForAnImage = int.parse(serverResponseString.substring(1));
                print("total line number: $totalLineNumberForAnImage");
                socket.write("6Connection Established");
              }
              else if (serverResponse == 4) {
                calculatedValue++;
                if(calculatedValue == totalLineNumberForAnImage){
                  isDrawing = false;
                  calculatedValue = 0;
                }
                socket.write("6Connection Established");
              }
              else if (serverResponse == 5) {
                socket.write("6Connection Established");
              }
              else if (serverResponse == 6) {
                socket.write("6Connection Established");
              }

          },
          onError: (error) {
            print('Error: $error');
            isDrawing = false;
            socket.close();
          },
          onDone: () {
            print('Server closed connection.');
            isDrawing = false;
            socket.close();
          },
        );
    } catch (e) {
      print('Bağlantı hatası: $e');
    }
  }

  Future getImage(ImageSource source) async {
    final picker = ImagePicker();
    final pickedFile = await picker.pickImage(    source: source,
        imageQuality: 10, // Adjust the image quality here (0-100)
    );
    String imagePath = ''; // New variable to hold the temporary file path

    setState(() {
      if (pickedFile != null) {
        _image = File(pickedFile.path);

        // Saving the image to a temporary location with the name "image.jpg"
        final Directory tempDir = Directory.systemTemp;
        final String fileName = 'image.jpg'; // Updated file name
        final File tempImage = File('${tempDir.path}/$fileName');
        tempImage.writeAsBytesSync(_image!.readAsBytesSync());
        imagePath = tempImage.path; // Updating the imagePath variable
        //draw = true;
      } else {
        print('No image selected.');
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      home: Scaffold(
        appBar: AppBar(
          title: Text('Image Page'),
        ),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              _image == null ? Text('Select an image to draw') : Image.file(_image!),
              SizedBox(height: 20),
              ElevatedButton(
                onPressed: () {
                  getImage(ImageSource.gallery);
                },
                child: Text('Gallery'),
              ),
              ElevatedButton(
                onPressed: () {
                  getImage(ImageSource.camera);
                },
                child: Text('Photo'),
              ),
            ],
          ),
        ),
        floatingActionButton: FloatingActionButton(
          onPressed: isDrawing ? null : () {
            setState(() {
              draw = true;
              isDrawing = true;
            });
          },
          child: Icon(Icons.draw),
          backgroundColor: (isDrawing || (_image == null && !isDrawing)) ? Color(0xFF4F4F4F) : Color(0xFF33C2FF),
          elevation: (isDrawing || (_image == null && !isDrawing)) ? 0 : 6, // Optional: To remove shadow when disabled

        ),
        floatingActionButtonLocation: FloatingActionButtonLocation.endFloat,
      ),
    );
  }
}
