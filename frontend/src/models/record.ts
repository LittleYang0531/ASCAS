interface WhereNodeLeaf {
    isLeaf: true
    column?: string
    op?: string
    value?: string
}

interface WhereNodeBranch {
    isLeaf: false
    isAnd?: boolean
    params?: Array<WhereNode>
}

export type WhereNode = WhereNodeLeaf | WhereNodeBranch;

export interface OrderNode {
    column: string
    isASC: boolean
}