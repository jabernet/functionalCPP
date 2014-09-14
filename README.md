With the new C++11 standard many powerful features for a powerful and less verbose type system were added to the C++ language. Together with lambda expressions they allow us to easily provide generic functional abstractions known from pure languages like Haskell. The code is still not as consise and generic as it's possible in Haskell, but it's quite close. And to make the methods more readable I provided the corresponding Haskell syntax type definitions. 

Methods implemented so far:

* `apply :: (a -> b) -> [a]` [1]
* `map :: (a -> b) -> [a] -> [b]`
* `foldr :: (a -> b -> b) -> b -> [a] -> b`
* `foldl :: (a -> b -> a) -> a -> [b] -> a`
* `zip :: [a] -> [b] -> [(a,b)]`
* `zipWith :: (a -> b -> c) -> [a] -> [b] -> [c]`
* `curry: ((a,b,...) -> c) -> (a -> b -> ... -> c)`
* `uncurry: (a -> b -> ... -> c) -> ((a,b,...) -> c)`

[1] Haskell doesn't have `apply`, since it's pure, but for C++ and other side-effect based languages it makes sense

More and hopefully some tests to come.
