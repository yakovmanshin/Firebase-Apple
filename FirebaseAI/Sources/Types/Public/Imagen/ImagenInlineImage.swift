// Copyright 2024 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

import Foundation

/// An image generated by Imagen, represented as inline data.
@available(iOS 15.0, macOS 12.0, macCatalyst 15.0, tvOS 15.0, watchOS 8.0, *)
public struct ImagenInlineImage: Sendable {
  /// The IANA standard MIME type of the image file; either `"image/png"` or `"image/jpeg"`.
  ///
  /// > Note: To request a different format, set ``ImagenGenerationConfig/imageFormat`` in
  ///   your ``ImagenGenerationConfig``.
  public let mimeType: String

  /// The image data in PNG or JPEG format.
  public let data: Data

  init(mimeType: String, data: Data) {
    self.mimeType = mimeType
    self.data = data
  }
}

@available(iOS 15.0, macOS 12.0, macCatalyst 15.0, tvOS 15.0, watchOS 8.0, *)
extension ImagenInlineImage: ImagenImageRepresentable {
  // TODO(andrewheard): Make this public when the SDK supports Imagen operations that take images as
  // input (upscaling / editing).
  var _internalImagenImage: _InternalImagenImage {
    _InternalImagenImage(
      mimeType: mimeType,
      bytesBase64Encoded: data.base64EncodedString(),
      gcsURI: nil
    )
  }
}

@available(iOS 15.0, macOS 12.0, macCatalyst 15.0, tvOS 15.0, watchOS 8.0, *)
extension ImagenInlineImage: Equatable {}

// MARK: - Codable Conformances

@available(iOS 15.0, macOS 12.0, macCatalyst 15.0, tvOS 15.0, watchOS 8.0, *)
extension ImagenInlineImage: Decodable {
  enum CodingKeys: CodingKey {
    case mimeType
    case bytesBase64Encoded
  }

  public init(from decoder: any Decoder) throws {
    let container = try decoder.container(keyedBy: CodingKeys.self)
    let mimeType = try container.decode(String.self, forKey: .mimeType)
    let bytesBase64Encoded = try container.decode(String.self, forKey: .bytesBase64Encoded)
    guard let data = Data(base64Encoded: bytesBase64Encoded) else {
      throw DecodingError.dataCorruptedError(
        forKey: .bytesBase64Encoded,
        in: container,
        debugDescription: "Failed to decode data from base64-encoded string: \(bytesBase64Encoded)"
      )
    }
    self.init(mimeType: mimeType, data: data)
  }
}
