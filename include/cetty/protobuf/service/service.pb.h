// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: cetty/protobuf/service/service.proto

#ifndef PROTOBUF_cetty_2fprotobuf_2fservice_2fservice_2eproto__INCLUDED
#define PROTOBUF_cetty_2fprotobuf_2fservice_2fservice_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
#include "google/protobuf/descriptor.pb.h"
// @@protoc_insertion_point(includes)

namespace cetty {
namespace protobuf {
namespace service {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_cetty_2fprotobuf_2fservice_2fservice_2eproto();
void protobuf_AssignDesc_cetty_2fprotobuf_2fservice_2fservice_2eproto();
void protobuf_ShutdownFile_cetty_2fprotobuf_2fservice_2fservice_2eproto();

class ServiceMessage;
class Empty;

enum MessageType {
  REQUEST = 1,
  RESPONSE = 2,
  ERROR = 3
};
bool MessageType_IsValid(int value);
const MessageType MessageType_MIN = REQUEST;
const MessageType MessageType_MAX = ERROR;
const int MessageType_ARRAYSIZE = MessageType_MAX + 1;

const ::google::protobuf::EnumDescriptor* MessageType_descriptor();
inline const ::std::string& MessageType_Name(MessageType value) {
  return ::google::protobuf::internal::NameOfEnum(
    MessageType_descriptor(), value);
}
inline bool MessageType_Parse(
    const ::std::string& name, MessageType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<MessageType>(
    MessageType_descriptor(), name, value);
}
enum ErrorCode {
  NO_ERROR = 0,
  WRONG_PROTO = 1,
  NO_SERVICE = 2,
  NO_METHOD = 3,
  INVALID_REQUEST = 4,
  INVALID_RESPONSE = 5
};
bool ErrorCode_IsValid(int value);
const ErrorCode ErrorCode_MIN = NO_ERROR;
const ErrorCode ErrorCode_MAX = INVALID_RESPONSE;
const int ErrorCode_ARRAYSIZE = ErrorCode_MAX + 1;

const ::google::protobuf::EnumDescriptor* ErrorCode_descriptor();
inline const ::std::string& ErrorCode_Name(ErrorCode value) {
  return ::google::protobuf::internal::NameOfEnum(
    ErrorCode_descriptor(), value);
}
inline bool ErrorCode_Parse(
    const ::std::string& name, ErrorCode* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ErrorCode>(
    ErrorCode_descriptor(), name, value);
}
// ===================================================================

class ServiceMessage : public ::google::protobuf::Message {
 public:
  ServiceMessage();
  virtual ~ServiceMessage();
  
  ServiceMessage(const ServiceMessage& from);
  
  inline ServiceMessage& operator=(const ServiceMessage& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const ServiceMessage& default_instance();
  
  void Swap(ServiceMessage* other);
  
  // implements Message ----------------------------------------------
  
  ServiceMessage* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ServiceMessage& from);
  void MergeFrom(const ServiceMessage& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required .cetty.protobuf.service.MessageType type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline cetty::protobuf::service::MessageType type() const;
  inline void set_type(cetty::protobuf::service::MessageType value);
  
  // required fixed64 id = 2;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 2;
  inline ::google::protobuf::uint64 id() const;
  inline void set_id(::google::protobuf::uint64 value);
  
  // optional string service = 3;
  inline bool has_service() const;
  inline void clear_service();
  static const int kServiceFieldNumber = 3;
  inline const ::std::string& service() const;
  inline void set_service(const ::std::string& value);
  inline void set_service(const char* value);
  inline void set_service(const char* value, size_t size);
  inline ::std::string* mutable_service();
  inline ::std::string* release_service();
  
  // optional string method = 4;
  inline bool has_method() const;
  inline void clear_method();
  static const int kMethodFieldNumber = 4;
  inline const ::std::string& method() const;
  inline void set_method(const ::std::string& value);
  inline void set_method(const char* value);
  inline void set_method(const char* value, size_t size);
  inline ::std::string* mutable_method();
  inline ::std::string* release_method();
  
  // optional .cetty.protobuf.service.ErrorCode error = 5;
  inline bool has_error() const;
  inline void clear_error();
  static const int kErrorFieldNumber = 5;
  inline cetty::protobuf::service::ErrorCode error() const;
  inline void set_error(cetty::protobuf::service::ErrorCode value);
  
  // optional bytes request = 6;
  inline bool has_request() const;
  inline void clear_request();
  static const int kRequestFieldNumber = 6;
  inline const ::std::string& request() const;
  inline void set_request(const ::std::string& value);
  inline void set_request(const char* value);
  inline void set_request(const void* value, size_t size);
  inline ::std::string* mutable_request();
  inline ::std::string* release_request();
  
  // optional bytes response = 7;
  inline bool has_response() const;
  inline void clear_response();
  static const int kResponseFieldNumber = 7;
  inline const ::std::string& response() const;
  inline void set_response(const ::std::string& value);
  inline void set_response(const char* value);
  inline void set_response(const void* value, size_t size);
  inline ::std::string* mutable_response();
  inline ::std::string* release_response();
  
  // @@protoc_insertion_point(class_scope:cetty.protobuf.service.ServiceMessage)
 private:
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_service();
  inline void clear_has_service();
  inline void set_has_method();
  inline void clear_has_method();
  inline void set_has_error();
  inline void clear_has_error();
  inline void set_has_request();
  inline void clear_has_request();
  inline void set_has_response();
  inline void clear_has_response();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint64 id_;
  ::std::string* service_;
  int type_;
  int error_;
  ::std::string* method_;
  ::std::string* request_;
  ::std::string* response_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(7 + 31) / 32];
  
  friend void  protobuf_AddDesc_cetty_2fprotobuf_2fservice_2fservice_2eproto();
  friend void protobuf_AssignDesc_cetty_2fprotobuf_2fservice_2fservice_2eproto();
  friend void protobuf_ShutdownFile_cetty_2fprotobuf_2fservice_2fservice_2eproto();
  
  void InitAsDefaultInstance();
  static ServiceMessage* default_instance_;
};
// -------------------------------------------------------------------

class Empty : public ::google::protobuf::Message {
 public:
  Empty();
  virtual ~Empty();
  
  Empty(const Empty& from);
  
  inline Empty& operator=(const Empty& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const Empty& default_instance();
  
  void Swap(Empty* other);
  
  // implements Message ----------------------------------------------
  
  Empty* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Empty& from);
  void MergeFrom(const Empty& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // @@protoc_insertion_point(class_scope:cetty.protobuf.service.Empty)
 private:
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[1];
  
  friend void  protobuf_AddDesc_cetty_2fprotobuf_2fservice_2fservice_2eproto();
  friend void protobuf_AssignDesc_cetty_2fprotobuf_2fservice_2fservice_2eproto();
  friend void protobuf_ShutdownFile_cetty_2fprotobuf_2fservice_2fservice_2eproto();
  
  void InitAsDefaultInstance();
  static Empty* default_instance_;
};
// ===================================================================

static const int kIdempotentFieldNumber = 1111;
extern ::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::MethodOptions,
    ::google::protobuf::internal::PrimitiveTypeTraits< bool >, 8, false >
  idempotent;
static const int kNoReturnFieldNumber = 1112;
extern ::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::MethodOptions,
    ::google::protobuf::internal::PrimitiveTypeTraits< bool >, 8, false >
  no_return;

// ===================================================================

// ServiceMessage

// required .cetty.protobuf.service.MessageType type = 1;
inline bool ServiceMessage::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ServiceMessage::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ServiceMessage::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ServiceMessage::clear_type() {
  type_ = 1;
  clear_has_type();
}
inline cetty::protobuf::service::MessageType ServiceMessage::type() const {
  return static_cast< cetty::protobuf::service::MessageType >(type_);
}
inline void ServiceMessage::set_type(cetty::protobuf::service::MessageType value) {
  GOOGLE_DCHECK(cetty::protobuf::service::MessageType_IsValid(value));
  set_has_type();
  type_ = value;
}

// required fixed64 id = 2;
inline bool ServiceMessage::has_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ServiceMessage::set_has_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ServiceMessage::clear_has_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ServiceMessage::clear_id() {
  id_ = GOOGLE_ULONGLONG(0);
  clear_has_id();
}
inline ::google::protobuf::uint64 ServiceMessage::id() const {
  return id_;
}
inline void ServiceMessage::set_id(::google::protobuf::uint64 value) {
  set_has_id();
  id_ = value;
}

// optional string service = 3;
inline bool ServiceMessage::has_service() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ServiceMessage::set_has_service() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ServiceMessage::clear_has_service() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ServiceMessage::clear_service() {
  if (service_ != &::google::protobuf::internal::kEmptyString) {
    service_->clear();
  }
  clear_has_service();
}
inline const ::std::string& ServiceMessage::service() const {
  return *service_;
}
inline void ServiceMessage::set_service(const ::std::string& value) {
  set_has_service();
  if (service_ == &::google::protobuf::internal::kEmptyString) {
    service_ = new ::std::string;
  }
  service_->assign(value);
}
inline void ServiceMessage::set_service(const char* value) {
  set_has_service();
  if (service_ == &::google::protobuf::internal::kEmptyString) {
    service_ = new ::std::string;
  }
  service_->assign(value);
}
inline void ServiceMessage::set_service(const char* value, size_t size) {
  set_has_service();
  if (service_ == &::google::protobuf::internal::kEmptyString) {
    service_ = new ::std::string;
  }
  service_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ServiceMessage::mutable_service() {
  set_has_service();
  if (service_ == &::google::protobuf::internal::kEmptyString) {
    service_ = new ::std::string;
  }
  return service_;
}
inline ::std::string* ServiceMessage::release_service() {
  clear_has_service();
  if (service_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = service_;
    service_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string method = 4;
inline bool ServiceMessage::has_method() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ServiceMessage::set_has_method() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ServiceMessage::clear_has_method() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ServiceMessage::clear_method() {
  if (method_ != &::google::protobuf::internal::kEmptyString) {
    method_->clear();
  }
  clear_has_method();
}
inline const ::std::string& ServiceMessage::method() const {
  return *method_;
}
inline void ServiceMessage::set_method(const ::std::string& value) {
  set_has_method();
  if (method_ == &::google::protobuf::internal::kEmptyString) {
    method_ = new ::std::string;
  }
  method_->assign(value);
}
inline void ServiceMessage::set_method(const char* value) {
  set_has_method();
  if (method_ == &::google::protobuf::internal::kEmptyString) {
    method_ = new ::std::string;
  }
  method_->assign(value);
}
inline void ServiceMessage::set_method(const char* value, size_t size) {
  set_has_method();
  if (method_ == &::google::protobuf::internal::kEmptyString) {
    method_ = new ::std::string;
  }
  method_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ServiceMessage::mutable_method() {
  set_has_method();
  if (method_ == &::google::protobuf::internal::kEmptyString) {
    method_ = new ::std::string;
  }
  return method_;
}
inline ::std::string* ServiceMessage::release_method() {
  clear_has_method();
  if (method_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = method_;
    method_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional .cetty.protobuf.service.ErrorCode error = 5;
inline bool ServiceMessage::has_error() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void ServiceMessage::set_has_error() {
  _has_bits_[0] |= 0x00000010u;
}
inline void ServiceMessage::clear_has_error() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void ServiceMessage::clear_error() {
  error_ = 0;
  clear_has_error();
}
inline cetty::protobuf::service::ErrorCode ServiceMessage::error() const {
  return static_cast< cetty::protobuf::service::ErrorCode >(error_);
}
inline void ServiceMessage::set_error(cetty::protobuf::service::ErrorCode value) {
  GOOGLE_DCHECK(cetty::protobuf::service::ErrorCode_IsValid(value));
  set_has_error();
  error_ = value;
}

// optional bytes request = 6;
inline bool ServiceMessage::has_request() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void ServiceMessage::set_has_request() {
  _has_bits_[0] |= 0x00000020u;
}
inline void ServiceMessage::clear_has_request() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void ServiceMessage::clear_request() {
  if (request_ != &::google::protobuf::internal::kEmptyString) {
    request_->clear();
  }
  clear_has_request();
}
inline const ::std::string& ServiceMessage::request() const {
  return *request_;
}
inline void ServiceMessage::set_request(const ::std::string& value) {
  set_has_request();
  if (request_ == &::google::protobuf::internal::kEmptyString) {
    request_ = new ::std::string;
  }
  request_->assign(value);
}
inline void ServiceMessage::set_request(const char* value) {
  set_has_request();
  if (request_ == &::google::protobuf::internal::kEmptyString) {
    request_ = new ::std::string;
  }
  request_->assign(value);
}
inline void ServiceMessage::set_request(const void* value, size_t size) {
  set_has_request();
  if (request_ == &::google::protobuf::internal::kEmptyString) {
    request_ = new ::std::string;
  }
  request_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ServiceMessage::mutable_request() {
  set_has_request();
  if (request_ == &::google::protobuf::internal::kEmptyString) {
    request_ = new ::std::string;
  }
  return request_;
}
inline ::std::string* ServiceMessage::release_request() {
  clear_has_request();
  if (request_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = request_;
    request_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional bytes response = 7;
inline bool ServiceMessage::has_response() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void ServiceMessage::set_has_response() {
  _has_bits_[0] |= 0x00000040u;
}
inline void ServiceMessage::clear_has_response() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void ServiceMessage::clear_response() {
  if (response_ != &::google::protobuf::internal::kEmptyString) {
    response_->clear();
  }
  clear_has_response();
}
inline const ::std::string& ServiceMessage::response() const {
  return *response_;
}
inline void ServiceMessage::set_response(const ::std::string& value) {
  set_has_response();
  if (response_ == &::google::protobuf::internal::kEmptyString) {
    response_ = new ::std::string;
  }
  response_->assign(value);
}
inline void ServiceMessage::set_response(const char* value) {
  set_has_response();
  if (response_ == &::google::protobuf::internal::kEmptyString) {
    response_ = new ::std::string;
  }
  response_->assign(value);
}
inline void ServiceMessage::set_response(const void* value, size_t size) {
  set_has_response();
  if (response_ == &::google::protobuf::internal::kEmptyString) {
    response_ = new ::std::string;
  }
  response_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ServiceMessage::mutable_response() {
  set_has_response();
  if (response_ == &::google::protobuf::internal::kEmptyString) {
    response_ = new ::std::string;
  }
  return response_;
}
inline ::std::string* ServiceMessage::release_response() {
  clear_has_response();
  if (response_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = response_;
    response_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// -------------------------------------------------------------------

// Empty


// @@protoc_insertion_point(namespace_scope)

}  // namespace service
}  // namespace protobuf
}  // namespace cetty

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< cetty::protobuf::service::MessageType>() {
  return cetty::protobuf::service::MessageType_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< cetty::protobuf::service::ErrorCode>() {
  return cetty::protobuf::service::ErrorCode_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_cetty_2fprotobuf_2fservice_2fservice_2eproto__INCLUDED


// Local Variables:
// mode: c++
// End:
