package joint

import "fmt"
import "sync"

type objectsMapType map[Internal_ComponentHandle]Internal_Accessor

var objectsMap struct {
    sync.Mutex
    objects objectsMapType
    idGenerator Internal_ComponentHandle
}

func Internal_RegisterObject(v Internal_Accessor) Internal_ComponentHandle {
    objectsMap.Lock()
    defer objectsMap.Unlock()
    if objectsMap.objects == nil {
        objectsMap.objects = make(objectsMapType)
    }
    objectsMap.idGenerator++
    ret := objectsMap.idGenerator
    objectsMap.objects[ret] = v
    //fmt.Printf("// registerObject: %d\n", ret)
    return ret
}

func Internal_GetObject(id Internal_ComponentHandle) Internal_Accessor {
    objectsMap.Lock()
    defer objectsMap.Unlock()
    ret := objectsMap.objects[id]
    if ret == nil {
        panic(fmt.Sprintf("Object %d not found", id))
    }
    return ret
}

func Internal_UnregisterObject(id Internal_ComponentHandle) {
    //fmt.Printf("// unregisterObject: %d\n", id)
    objectsMap.Lock()
    defer objectsMap.Unlock()
    if objectsMap.objects[id] == nil {
        panic(fmt.Sprintf("Object %d not found", id))
    }
    delete(objectsMap.objects, id)
}
