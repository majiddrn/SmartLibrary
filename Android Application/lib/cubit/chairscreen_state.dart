part of 'chairscreen_cubit.dart';

@immutable
sealed class ChairscreenState {}

final class ChairscreenInitial extends ChairscreenState {}
final class Loading extends ChairscreenState {}

final class Success extends ChairscreenState {}
final class Notif extends ChairscreenState {
Notif({required this.message});
  final String message;
}
// final class ShowMessage extends ChairscreenState {}
final class Empty extends ChairscreenState {}
final class Error extends ChairscreenState {
  final Function onCall;
  final String error;
  Error({required this.onCall , required this.error});
}
