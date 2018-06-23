-- Example demonstrating how to manipulate complex data structures
-- (called tables) and convert them back and forth to JSON strings

-- prepare the inspection module to print contents of complex data
-- structures, using i.print() instead of print()
i = require "inspect"

-- take an example JSON string
DATA = [[
{
  "squadName": "Super hero squad",
  "homeTown": "Metro City",
  "formed": 2016,
  "secretBase": "Super tower",
  "active": true,
  "members": [
    {
      "name": "Molecule Man",
      "age": 29,
      "secretIdentity": "Dan Jukes",
      "powers": [
        "Radiation resistance",
        "Turning tiny",
        "Radiation blast"
      ]
    },
    {
      "name": "Madame Uppercut",
      "age": 39,
      "secretIdentity": "Jane Wilson",
      "powers": [
        "Million tonne punch",
        "Damage resistance",
        "Superhuman reflexes"
      ]
    },
    {
      "name": "Eternal Flame",
      "age": 1000000,
      "secretIdentity": "Unknown",
      "powers": [
        "Immortality",
        "Heat Immunity",
        "Inferno",
        "Teleportation",
        "Interdimensional travel"
      ]
    }
  ]
}
]]

-- this converts the JSON string to a table on which various
-- operations can be done (see @tables and @functions modules)
superheroes = json.decode(DATA)

-- iterate through the members array and print out only names
for k,v in ipairs(superheroes.members) do
   i.print(fun.at(v,"name"))
end

-- the previous approach is procedural,
-- let's do the same time in a functional way using function chaining!
i.print(
   fun.chain(superheroes.members)
   :pluck("name")
   :value()
)
-- this constitutes a base for further evolution of the zenroom
-- language into declarative after all use-casesare well examined.

-- print out the json
print(
   json.encode(superheroes.members)
)
