# [The Cryptopals Crypto Challenges](https://cryptopals.com)

## Welcome to the challenges

We can't introduce these any better than [Maciej Ceglowski](https://blog.pinboard.in/2013/04/the_matasano_crypto_challenges/) did, so read that blog post first.

We've built a collection of 48 exercises that demonstrate attacks on real-world crypto.

This is a different way to learn about crypto than taking a class or reading a book. We give you problems to solve. They're derived from weaknesses in real-world systems and modern cryptographic constructions. We give you enough info to learn about the underlying crypto concepts yourself. When you're finished, you'll not only have learned a good deal about how cryptosystems are built, but you'll also understand how they're attacked.

### What Are The Rules?

There aren't any! For several years, we ran these challenges over email, and asked participants not to share their results. The honor system worked beautifully! But now we're ready to set aside the ceremony and just publish the challenges for everyone to work on.

### How Much Math Do I Need To Know?

If you have any trouble with the math in these problems, you should be able to find a local 9th grader to help you out. It turns out that many modern crypto attacks don't involve much hard math.

### How Much Crypto Do I Need To Know?

None. That's the point.

### So What Do I Need To Know?

You'll want to be able to code proficiently in any language. We've received submissions in C, C++, Python, Ruby, Perl, Visual Basic, X86 Assembly, Haskell, and Lisp. Surprise us with another language. Our friend Maciej says these challenges are a good way to learn a new language, so maybe now's the time to pick up Clojure or Rust.

### What Should I Expect?

Right now, we have eight sets. They get progressively harder. Again: these are based off real-world vulnerabilities. None of them are "puzzles". They're not designed to trip you up. Some of the attacks are clever, though, and if you're not familiar with crypto cleverness... well, you should like solving puzzles. An appreciation for early-90's MTV hip-hop can't hurt either.

### Can You Give Us A Long-Winded Indulgent Description For Why You'Ve Chosen To Do This?

_It turns out that we can._

If you're not that familiar with crypto already, or if your familiarity comes mostly from things like Applied Cryptography, this fact may surprise you: most crypto is fatally broken. The systems we're relying on today that aren't known to be fatally broken are in a state of just waiting to be fatally broken. Nobody is sure that TLS 1.2 or SSH 2 or OTR are going to remain safe as designed.

The current state of crypto software security is similar to the state of software security in the 1990s. Specifically: until around 1995, it was not common knowledge that software built by humans might have trouble counting. As a result, nobody could size a buffer properly, and humanity incurred billions of dollars in cleanup after a decade and a half of emergency fixes for memory corruption vulnerabilities.

Counting is not a hard problem. But cryptography is. There are just a few things you can screw up to get the size of a buffer wrong. There are tens, probably hundreds, of obscure little things you can do to take a cryptosystem that should be secure even against an adversary with more CPU cores than there are atoms in the solar system, and make it solveable with a Perl script and 15 seconds. Don't take our word for it: do the challenges and you'll see.

People "know" this already, but they don't really know it in their gut, and we think the reason for that is that very few people actually know how to implement the best-known attacks. So, mail us, and we'll give you a tour of them.

### How do I start?

Start here!

### Who did this?

  * Thomas Ptacek (@tqbf)
  * Sean Devlin (@spdevlin)
  * Alex Balducci (@iamalexalright)
  * Marcin Wielgoszewski (@marcinw)

Cryptopals is maintained and expanded (from Set 8 on) by Sean Devlin, in conjunction with the Cryptography Services Team at NCC Group.

We could not possibly have done this without the help of several other people. Roughly in order of influence:

  * Nate Lawson taught us virtually everything we know about cryptography.
  * Trevor Perrin taught Nate some of that. I can tell you a pretty compelling story about how Trevor is the intellectual origin of every successful attack on TLS over the past 5 years.
  * Thai Duong and Juliano Rizzo are the godfathers of practical cryptographic software security. Several things in this challenge didn't make sense to us until after Thai and Juliano exploited them in mainstream software.

## [Set 1: Basics](https://cryptopals.com/sets/1)

This is the **qualifying set**. We picked the exercises in it to ramp developers up gradually into coding cryptography, but also to verify that we were working with people who were ready to write code.

This set is **relatively easy**. With one exception, most of these exercises should take only a couple minutes. But don't beat yourself up if it takes longer than that. It took Alex two weeks to get through the set!

If you've written any crypto code in the past, you're going to feel like skipping a lot of this. **Don't skip them**. At least two of them (we won't say which) are important stepping stones to later attacks.

  1. [Convert hex to base64](cc-01.md)
  2. [Fixed XOR](cc-02.md)
  3. [Single-byte XOR cipher](cc-03.md)
  4. [Detect single-character XOR](cc-04.md)
  5. [Implement repeating-key XOR](cc-05.md)
  6. [Break repeating-key XOR](cc-06.md)
  7. [AES in ECB mode](cc-07.md)
  8. [Detect AES in ECB mode](cc-08.md)


## [Set 2: Block crypto](https://cryptopals.com/sets/2)

This is the first of several sets on **block cipher cryptography**. This is bread-and-butter crypto, the kind you'll see implemented in most web software that does crypto.

This set is **relatively easy**. People that clear set 1 tend to clear set 2 somewhat quickly.

Three of the challenges in this set are extremely valuable in breaking real-world crypto; one allows you to decrypt messages encrypted in the default mode of AES, and the other two allow you to rewrite messages encrypted in the most popular modes of AES.

   9. [Implement PKCS# 7 padding](cc-09.md)
  10. [Implement CBC mode](cc-10.md)
  11. [An ECB/CBC detection oracle](cc-11.md)
  12. [Byte-at-a-time ECB decryption (Simple)](cc-12.md)
  13. [ECB cut-and-paste](cc-13.md)
  14. [Byte-at-a-time ECB decryption (Harder)](cc-14.md)
  15. [PKCS#7 padding validation](cc-15.md)
  16. [CBC bitflipping attacks](cc-16.md)


## [Set 3: Block & stream crypto](https://cryptopals.com/sets/3)

This is the next set of **block cipher cryptography** challenges (even the randomness stuff here plays into block cipher crypto).

This set is **moderately difficult**. It includes a famous attack against CBC mode, and a "cloning" attack on a popular RNG that can be annoying to get right.

We've also reached a point in the crypto challenges where all the challenges, with one possible exception, are valuable in breaking real-world crypto.

  17. [The CBC padding oracle](cc-17.md)
  18. [Implement CTR, the stream cipher mode](cc-18.md)
  19. [Break fixed-nonce CTR mode using substitutions](cc-19.md)
  20. [Break fixed-nonce CTR statistically](cc-20.md)
  21. [Implement the MT19937 Mersenne Twister RNG](cc-21.md)
  22. [Crack an MT19937 seed](cc-22.md)
  23. [Clone an MT19937 RNG from its output](cc-23.md)
  24. [Create the MT19937 stream cipher and break it](cc-24.md)


## [Set 4: Stream crypto and randomness](https://cryptopals.com/sets/4)

This is the last set of **block cipher cryptography** challenges, and also our coverage of message authentication.

This set is **much easier** than the last set. We introduce some new concepts, but the attacks themselves involve less code than, say, the CBC padding oracle.

Things get significantly trickier in the next two sets. A lot of people drop off after set 4.

  25. [Break "random access read/write" AES CTR](cc-25.md)
  26. [CTR bitflipping](cc-26.md)
  27. [Recover the key from CBC with IV=Key](cc-27.md)
  28. [Implement a SHA-1 keyed MAC](cc-28.md)
  29. [Break a SHA-1 keyed MAC using length extension](cc-29.md)
  30. [Break an MD4 keyed MAC using length extension](cc-30.md)
  31. [Implement and break HMAC-SHA1 with an artificial timing leak](cc-31.md)
  32. [Break HMAC-SHA1 with a slightly less artificial timing leak](cc-32.md)


## [Set 5: Diffie-Hellman and friends](https://cryptopals.com/sets/5)

This is the first set of **number-theoretic cryptography** challenges, and also our coverage of message authentication.

This set is **significantly harder** than the last set. The concepts are new, the attacks bear no resemblance to those of the previous sets, and... math.

On the other hand, **our favorite cryptanalytic attack ever** is in this set (you'll see it soon). We're happy with this set. Don't wimp out here. You're almost done!

  33. [Implement Diffie-Hellman](cc-33.md)
  34. [Implement a MITM key-fixing attack on Diffie-Hellman with parameter injection](cc-34.md)
  35. [Implement DH with negotiated groups, and break with malicious "g" parameters](cc-35.md)
  36. [Implement Secure Remote Password (SRP)](cc-36.md)
  37. [Break SRP with a zero key](cc-37.md)
  38. [Offline dictionary attack on simplified SRP](cc-38.md)
  39. [Implement RSA](cc-39.md)
  40. [Implement an E=3 RSA Broadcast attack](cc-40.md)


## [Set 6: RSA and DSA](https://cryptopals.com/sets/6)

This is **the last of our original crypto challenges**.

This set exclusively covers **number-theoretic cryptography**, and, in particular, RSA and DSA.

This set is **hard**. The concepts are again new. The attacks involve some math --- but nothing you didn't learn in 9th grade --- and a significant amount of programming.

But **they're worth it**. Two of these attacks in particular are among the most valuable in real-world cryptography.

  41. [Implement unpadded message recovery oracle](cc-41.md)
  42. [Bleichenbacher's e=3 RSA Attack](cc-42.md)
  43. [DSA key recovery from nonce](cc-43.md)
  44. [DSA nonce recovery from repeated nonce](cc-44.md)
  45. [DSA parameter tampering](cc-45.md)
  46. [RSA parity oracle](cc-46.md)
  47. [Bleichenbacher's PKCS 1.5 Padding Oracle (Simple Case)](cc-47.md)
  48. [Bleichenbacher's PKCS 1.5 Padding Oracle (Complete Case)](cc-48.md)


## [Set 7: Hashes](https://cryptopals.com/sets/7)

This is **the first of two sets we generated after the original 6**.

Unlike the last few sets, **this set is a hodge-podge**. It also includes some of the few challenges we have that probably aren't useful against real targets (they were fun enough to include anyways). On the other hand, we also include a challenge that models the CRIME attack on TLS.

This set is **hard**. There's a significant amount of programming, and Wang's attack in particular is as difficult as anything we've done.

  49. [CBC-MAC Message Forgery](cc-49.md)
  50. [Hashing with CBC-MAC](cc-50.md)
  51. [Compression Ratio Side-Channel Attacks](cc-51.md)
  52. [Iterated Hash Function Multicollisions](cc-52.md)
  53. [Kelsey and Schneier's Expandable Messages](cc-53.md)
  54. [Kelsey and Kohno's Nostradamus Attack](cc-54.md)
  55. [MD4 Collisions](cc-55.md)
  56. [RC4 Single-Byte Biases](cc-56.md)


## [Set 8: Abstract Algebra](https://cryptopals.com/sets/8)

This is **the second of two sets we generated after the original 6**.

This set focuses on abstract algebra, including DH, GCM, and (most importantly) elliptic curve cryptography. Fair warning - it's really tough! There's a ton of content here, and it's more demanding than anything we've released so far. By the time you're done, you will have written an ad hoc, informally-specified, bug-ridden, slow implementation of one percent of SageMath.

  57. [Diffie-Hellman Revisited: Small Subgroup Confinement](cc-57.md)
  58. [Pollard's Method for Catching Kangaroos](cc-58.md)
  59. [Elliptic Curve Diffie-Hellman and Invalid-Curve Attacks](cc-59.md)
  60. [Single-Coordinate Ladders and Insecure Twists](cc-60.md)
  61. [Duplicate-Signature Key Selection in ECDSA (and RSA)](cc-61.md)
  62. [Key-Recovery Attacks on ECDSA with Biased Nonces](cc-62.md)
  63. [Key-Recovery Attacks on GCM with Repeated Nonces](cc-63.md)
  64. [Key-Recovery Attacks on GCM with a Truncated MAC](cc-64.md)

But wait, where are the actual challenges?! You'll need to mail in for them, just like in the original cryptopals. Send a mail to set#.cryptopals@gmail.com and make the subject "Crazy Flamboyant for the Rap Enjoyment". BUT replace the # with the number of the set you want. Which would be 8. So replace it with 8.
