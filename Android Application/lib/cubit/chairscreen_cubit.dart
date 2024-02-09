import 'dart:convert';

import 'package:bloc/bloc.dart';
import 'package:meta/meta.dart';
import 'package:http/http.dart' as http;
import 'package:remotea_pplication/consts.dart';
part 'chairscreen_state.dart';

class ChairscreenCubit extends Cubit<ChairscreenState> {
  ChairscreenCubit({required this.librariId}) : super(ChairscreenInitial()) {
    fetchcChairs();
  }
  int librariId;
  late List<LibraryItem> chairs;
  List<Map<String, dynamic>> notifs = [];
  void fetchNotif() async {
    try {
      // Perform the network request
      final response = await http.get(
        Uri.parse(Utils.getNotif),
        // body: jsonEncode({"studentCode": ""}),
        headers: {
          "Content-Type": "application/json",
          'Authorization': 'Bearer ${Utils.token}',
        },
      );
      if (response.statusCode == 200) {
        notifs.clear();
        List<dynamic> newList = jsonDecode(response.body);
        // print(newList.length);
        // Map<String, dynamic> json = jsonDecode(response.body);
        newList.forEach((element) {
          notifs.add((element as Map<String, dynamic>));
        });
       
      //  print(DateTime.now().difference(time).inSeconds);
        // print("object");
        if (notifs.isNotEmpty) {
          DateTime time= DateTime.parse(notifs.last["Date"]);
          if(DateTime.now().difference(time).inSeconds <120){
print(DateTime.now().difference(time).inSeconds);
emit(Notif(message: "please get back to your seat"));
          }else{
   emit(Notif(message: "there is no notification"));

          }
          // if(notifs.last["Date"]);
          
        } else {
          emit(Notif(message: "there is no notification"));
        }
        // print(decodedToken);
        // Handle successful response
        // return null;
      } else {
        // emit(Error(
        //     error: response.body,
        //     onCall: () {
        //       fetchcChairs();
        //     }));
        // Handle server error and display appropriate message
        // return response.body??'An error occurred. Please try again later.';
      }
    } catch (e) {
      emit(Error(
          error: e.toString(),
          onCall: () {
            fetchcChairs();
          }));
      // Handle other errors, such as network issues
      // print('Error: $e');
      // return 'An error occurred. Please try again later.';
    }
  }

  void empty() {
    emit(Empty());
  }

  void fetchcChairs() async {
    emit(Loading());
    try {
      // Perform the network request
      final response = await http.get(
        Uri.parse(Utils.getChairs + "?id=" + librariId.toString()),
        // body: jsonEncode({"studentCode": ""}),
        headers: {"Content-Type": "application/json"},
      );
      if (response.statusCode == 200) {
        Map<String, dynamic> json = jsonDecode(response.body);
        Library library = Library.fromJson(json['library']);
        chairs = library.libraryItems;

        emit(Success());
        // print(decodedToken);
        // Handle successful response
        // return null;
      } else {
        emit(Error(
            error: response.body,
            onCall: () {
              fetchcChairs();
            }));
        // Handle server error and display appropriate message
        // return response.body??'An error occurred. Please try again later.';
      }
    } catch (e) {
      emit(Error(
          error: e.toString(),
          onCall: () {
            fetchcChairs();
          }));
      // Handle other errors, such as network issues
      // print('Error: $e');
      // return 'An error occurred. Please try again later.';
    }
  }
}

class LibraryItem {
  String title;
  int status;
  int libraryId;

  LibraryItem({
    required this.title,
    required this.status,
    required this.libraryId,
  });

  factory LibraryItem.fromJson(Map<String, dynamic> json) {
    return LibraryItem(
      title: json['Title'],
      status: json['Status'],
      libraryId: json['LibraryId'],
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'Title': title,
      'Status': status,
      'LibraryId': libraryId,
    };
  }
}

class Library {
  int id;
  String title;
  bool isOnline;
  List<LibraryItem> libraryItems;

  Library({
    required this.id,
    required this.title,
    required this.isOnline,
    required this.libraryItems,
  });

  factory Library.fromJson(Map<String, dynamic> json) {
    return Library(
      id: json['Library']['Id'],
      title: json['Library']['Title'],
      isOnline: json['Library']['IsOnline'],
      libraryItems: (json['LibraryItems'] as List)
          .map((itemJson) => LibraryItem.fromJson(itemJson))
          .toList(),
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'Library': {
        'Id': id,
        'Title': title,
        'IsOnline': isOnline,
      },
      'LibraryItems': libraryItems.map((item) => item.toJson()).toList(),
    };
  }
}
