# object-Coder

Task: object Coder
1. Based on the Coder class, implement a simple permutation cipher encoder/decoder
> Key field - private
> A constructor without parameters generates a random key
> A constructor with parameters creates an object with the given key
> The decode and encode methods should perform decryption and encryption respectively
> Examples of using class methods outside the class:
  Coder object;
  std::string text;
  syd::cin >> text;
  text = object.encode(text);
  std::cout << "Encoded text: " << text << std::endl;
  std::cout << "Decoded text: " << object.decode(text) << std::endl;

2. Based on the Coder class, implement an object
capable of creating a secure channel with an object of the same type.
> In addition to the private field secret_key or private_key, use the public field public_key
> ALL secret fields must be private
> ALL public keys must be public
> Use one of the public key ciphers to create a secure channel.
1. Diffie-Hellman
2. Shamir
3. Dl-Gamal
4. Implement the same on RSA.
