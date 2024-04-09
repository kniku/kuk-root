import glob
import json
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--package', '-p', required=True)
parser.add_argument('--target', '-t', required=False)
parser.add_argument('--details', '-d', required=False, action='store_true')
args = parser.parse_args()

def findDeps(target, key2find, dictJson):
    # print('#searching', target, ':', key2find)
    deps = []
    for package in dictJson['targets'][target]:
        if 'dependencies' in dictJson['targets'][target][package]:
            for key in dictJson['targets'][target][package]['dependencies']:
                versionKey = key+'/'+dictJson['targets'][target][package]['dependencies'][key]
                if key2find == key or key2find == versionKey:
                    # print(key2find, '..found', package, '=>', versionKey)
                    xdeps = findDeps(target, package, dictJson)
                    if len(xdeps) == 0:
                        # print(key2find, '....found', package, '=>', versionKey)
                        deps.append([package, versionKey])
                    else:
                        for xdep in xdeps:
                            # print(key2find, '......found', xdep[0], '=>', package,'|',xdep[1], '=>', versionKey)
                            deps.append(xdep + [versionKey])

    return deps

try:
    # print(args.package)
    for fn in glob.glob('./**/project.assets.json', recursive=True):
        f = open(fn, 'rt')
        data = json.load(f)
        f.close()

        for target in data['targets']:
            # print('Target:', target)
            if not args.details or (args.target and args.target != target):
                continue

            deps = findDeps(target, args.package, data)

            if len(deps) > 0:
                print(fn, '('+target+')')
                for dep in deps:
                    print('  ', ' => '.join(dep))

        for lib in data['libraries']:
            if args.package+'/' in lib:
                print('>>>>>', fn, '=>', lib)
          
except Exception as err:
	print('ERR:', err)

